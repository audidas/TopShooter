// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable  UMETA(DisplayName = "Consumable"),
	Weapon      UMETA(DisplayName = "Weapon"),
	Equipment   UMETA(DisplayName = "Equipment"),
	Ammo        UMETA(DisplayName = "Ammo"),
	Misc        UMETA(DisplayName = "Misc")
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TOPSHOOTEREXAMPLE_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	FText ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info", meta = (MultiLine = true))
	FText ItemDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	UTexture2D* Icon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	EItemType ItemType;
	
	// 중첩 여부 ( 1 = 중첩 X )
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Logic")
	int32 MaxStackSize = 1;
	
	// 소모품 사용 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Logic")
	float UseTime = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Logic")
	TSubclassOf<AActor> ItemActorClass;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemData", GetFName());	
	};
};

USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UItemData* ItemData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Quantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float CurrentDurability;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsSearched;
	
	FItemStack()
		: ItemData(nullptr), Quantity(0), CurrentDurability(-1.0f), bIsSearched(true) {}
	
	bool IsValid() const { return ItemData != nullptr && Quantity > 0; }
};
