// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Utils/InventoryComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	Capacity = 14;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Items.SetNum(Capacity);
	EquipmentItems.SetNum(5);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction*
                                        ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FItemStack UInventoryComponent::EquipItem(int32 SlotIndex, FItemStack NewItem)
{
	if (!EquipmentItems.IsValidIndex(SlotIndex)) return NewItem;
	
	FItemStack PreviousItem = EquipmentItems[SlotIndex];
	EquipmentItems[SlotIndex] = NewItem;
	
	return PreviousItem;
}

FItemStack UInventoryComponent::UnequipItem(int32 SlotIndex)
{
	if (!EquipmentItems.IsValidIndex(SlotIndex)) return FItemStack();
	
	FItemStack RemovedItem = EquipmentItems[SlotIndex];
	EquipmentItems[SlotIndex] = FItemStack();
	
	return RemovedItem;
}

int32 UInventoryComponent::AddItem(UItemData* NewItem, int32 Amount)
{
	if (!NewItem || Amount <= 0) return Amount;
	
	int32 LeftoverAmount = Amount;
	
	if (NewItem->MaxStackSize > 1)
	{
		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (Items[i].IsValid() && Items[i].ItemData == NewItem && Items[i].Quantity < NewItem->MaxStackSize)
			{
				int32 SpaceRemaining = NewItem->MaxStackSize - Items[i].Quantity;
				int32 AmountToAdd = FMath::Min(LeftoverAmount, SpaceRemaining);

				Items[i].Quantity += AmountToAdd;
				LeftoverAmount -= AmountToAdd;

				if (LeftoverAmount <= 0) break; 
			}
		}
	}
	
	if (LeftoverAmount > 0)
	{
		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (!Items[i].IsValid())
			{
				int32 AmountToAdd = FMath::Min(LeftoverAmount, NewItem->MaxStackSize);

				Items[i].ItemData = NewItem;
				Items[i].Quantity = AmountToAdd;
				
				Items[i].CurrentDurability = (NewItem->ItemType == EItemType::Equipment) ? 100.0f : -1.0f; 

				LeftoverAmount -= AmountToAdd;

				if (LeftoverAmount <= 0) break;
			}
		}
	}
	
	if (LeftoverAmount < Amount)
	{
		OnInventoryUpdated.Broadcast();
	}
	
	return LeftoverAmount;
}

FItemStack UInventoryComponent::GetItemAtIndex(int32 Index) const
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index];
	}
	return FItemStack();
	
}

void UInventoryComponent::RemoveItemAtIndex(int32 Index, int32 RemoveAmount)
{
	if (!Items.IsValidIndex(Index) || !Items[Index].IsValid()) return;

	Items[Index].Quantity -= RemoveAmount;

	if (Items[Index].Quantity <= 0)
	{
		
		Items[Index] = FItemStack();
	}

	OnInventoryUpdated.Broadcast();
	
}

void UInventoryComponent::SwapOrMoveItem(int32 SourceIndex,
	int32 DestinationIndex)
{
	if (!Items.IsValidIndex(SourceIndex) || !Items.IsValidIndex(DestinationIndex)) return;
	if (SourceIndex == DestinationIndex) return;
	
	FItemStack& SourceItem = Items[SourceIndex];
	FItemStack& DestItem = Items[DestinationIndex];
	
	if (SourceItem.ItemData == nullptr) return;
	
	if (DestItem.ItemData != nullptr && 
		DestItem.ItemData == SourceItem.ItemData && 
		DestItem.ItemData->MaxStackSize > 1)
	{
		int32 MaxStack = DestItem.ItemData->MaxStackSize;
		int32 SpaceRemaining = MaxStack - DestItem.Quantity;
		
		if (SpaceRemaining > 0)
		{
			int32 AmountToMove = FMath::Min(SourceItem.Quantity, SpaceRemaining);

			DestItem.Quantity += AmountToMove;
			SourceItem.Quantity -= AmountToMove;
			
			if (SourceItem.Quantity <= 0)
			{
				SourceItem = FItemStack();
			}
			if (OnInventoryUpdated.IsBound()) OnInventoryUpdated.Broadcast();
			return;
		}
	}
	Swap(SourceItem, DestItem);
	if (OnInventoryUpdated.IsBound())
	{
		OnInventoryUpdated.Broadcast();
	}
}

void UInventoryComponent::TransferItem(int32 SourceIndex, bool bFromEquipment,
	int32 DestIndex, bool bToEquipment)
{
	
	if (!bFromEquipment && !bToEquipment)
	{
		SwapOrMoveItem(SourceIndex, DestIndex);
		return;
	}
	
	if (bFromEquipment && bToEquipment)
	{
		if (EquipmentItems.IsValidIndex(SourceIndex) && EquipmentItems.IsValidIndex(DestIndex))
		{
			EquipmentItems.Swap(SourceIndex, DestIndex);
			OnInventoryUpdated.Broadcast();
		}
		return;
	}
	
	if (!bFromEquipment && bToEquipment)
	{
		if (!Items.IsValidIndex(SourceIndex) || !EquipmentItems.IsValidIndex(DestIndex)) return;
		
		FItemStack& BagItem = Items[SourceIndex];
		
		if (!BagItem.IsValid()) return;
		
		// TODO: 여기서 아이템 타입 검사
		// if (DestIndex == 0 && BagItem.ItemData->ItemType != EItemType::Weapon) return;
		
		FItemStack OldEquipItem = EquipmentItems[DestIndex];
		EquipmentItems[DestIndex] = BagItem;
		Items[SourceIndex] = OldEquipItem;
		
		OnInventoryUpdated.Broadcast();
		return;
	}
	
	if (bFromEquipment && !bToEquipment)
	{
		if (!EquipmentItems.IsValidIndex(SourceIndex) || !Items.IsValidIndex(DestIndex)) return;

		FItemStack& EquipItem = EquipmentItems[SourceIndex];
		FItemStack& BagItem = Items[DestIndex];

		// 빈 슬롯으로 옮기는 경우 or 아이템이 있는 곳으로 옮기는 경우(스왑)
		FItemStack Temp = BagItem;
		BagItem = EquipItem;
		EquipmentItems[SourceIndex] = Temp;

		OnInventoryUpdated.Broadcast();
		return;
	}
}

