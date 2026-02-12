// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemData.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPSHOOTEREXAMPLE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Capacity = 14;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FItemStack> Items;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FItemStack> EquipmentItems;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FItemStack EquipItem(int32 SlotIndex, FItemStack NewItem);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FItemStack UnequipItem(int32 SlotIndex);
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 AddItem(UItemData* NewItem, int32 Amount);
	
	UFUNCTION(BlueprintPure, Category = "Inventory")
	FItemStack GetItemAtIndex(int32 Index) const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemAtIndex(int32 Index, int32 RemoveAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwapOrMoveItem(int32 SourceIndex, int32 DestinationIndex);
};

