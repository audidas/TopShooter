// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


USTRUCT(BlueprintType)
struct FArmorData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReductionRate = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDurability = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentDurability = 0.0f;
	
	void TakeDurabilityDamage(float Amount)
	{
		CurrentDurability = FMath::Clamp(CurrentDurability - Amount, 0.0f, MaxDurability);
	}
	
	bool IsBroken() const { return CurrentDurability <= 0.0f; }
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZeroHealthDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedDelegate, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpDelegate, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExpChangedDelegate, float, Current, float, Max);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPSHOOTEREXAMPLE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;
	
public:
	// 체력
	// ----------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Health")
	float MaxHealth = 40.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Health")
	float CurrentHealth;
	
	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float DamageAmount, FName HitBoneName);
	
	UPROPERTY(BlueprintAssignable)
	FOnZeroHealthDelegate OnZeroHealth;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChanged;
	// ----------------------------------------------------
	
	// 방어도
	// ----------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Armor")
	FArmorData HeadArmor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Armor")
	FArmorData BodyArmor;
	
	UFUNCTION(BlueprintCallable)
	void EquipArmor(bool bIsHead,float ReductionRate, float Durability);
	// ----------------------------------------------------
	
	// 스태미너 관련  
	// ----------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Optimization")
	bool bEnableStamina = false; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Stamina", meta = (EditCondition = "bEnableStamina"))
	float MaxStamina = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Stamina")
	float CurrentStamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float StaminaRegenRate = 15.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float StaminaRegenDelay = 1.0f;
	
	UPROPERTY(BlueprintAssignable)
	FOnStaminaChangedDelegate OnStaminaChanged;
	
	UFUNCTION(BlueprintCallable)
	bool UseStamina(float Cost);
	// ----------------------------------------------------
	
	// 레벨 
	// ----------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Level")
	int32 CurrentLevel = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Level")
	float CurrentExp = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Level")
	float AcquiredExp = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Level")
	float NextExp = 100.0f;
	
	UPROPERTY(BlueprintAssignable)
	FOnLevelUpDelegate OnLevelUp;
	
	UPROPERTY(BlueprintAssignable)
	FOnExpChangedDelegate OnExpChanged;
	
	UFUNCTION(BlueprintCallable)
	void AddAcquiredExp(float Amount);
	
	UFUNCTION(BlueprintCallable)
	void ResolveExp(bool bIsEscaped);
	
	void ProcessLevelUp(float FinalAmount);
private:
	float LastStaminaUseTime;
};
