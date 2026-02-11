// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Utils/StatComponent.h"


// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	/*
	 * 기본값 설정
	 */
	
	// 체력 기본값 
	MaxHealth = 40.0f;
	CurrentHealth = MaxHealth;
	
	// 스태미너 기본값
	bEnableStamina = false;
	MaxStamina =100.0f;
	CurrentStamina = MaxStamina;
	StaminaRegenRate = 15.0f;
	StaminaRegenDelay = 1.5f;
	
	// 레벨
	CurrentLevel = 1;
	CurrentExp = 0.0f;
	NextExp = 100.0f;
	AcquiredExp = 0.0f;
	
	// 방어구
	HeadArmor = FArmorData();
	BodyArmor = FArmorData();
	
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	
	if (OnHealthChanged.IsBound()) OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	if (OnStaminaChanged.IsBound()) OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	
	// 스태미나를 안 쓰는 적(Enemy)들은 Tick off
	if (!bEnableStamina)
	{
		SetComponentTickEnabled(false);
	}
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                   FActorComponentTickFunction*
                                   ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bEnableStamina) return;
	if (CurrentStamina < MaxStamina && (GetWorld()->GetTimeSeconds() - LastStaminaUseTime) >= StaminaRegenDelay)
	{
		
		float PreviousStamina = CurrentStamina;
		CurrentStamina = FMath::Clamp(CurrentStamina + (StaminaRegenRate * DeltaTime), 0.0f, MaxStamina);
		
		if (!FMath::IsNearlyEqual(PreviousStamina, CurrentStamina))
		{
			OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
		}
	}
	
}

void UStatComponent::ApplyDamage(float DamageAmount, FName HitBoneName)
{
	
	if (CurrentHealth <= 0.0f) return;
	float FinalDamage = DamageAmount;
	
	bool bIsHeadHit = HitBoneName.ToString().Contains(TEXT("head"), ESearchCase::IgnoreCase);
	FArmorData* TargetArmor = bIsHeadHit ? &HeadArmor : &BodyArmor;
	
	if (TargetArmor->MaxDurability > 0.0f && !TargetArmor->IsBroken())
	{
		float Reduction = FMath::Clamp(TargetArmor->ReductionRate, 0.0f, 1.0f);
		FinalDamage *= (1.0f - Reduction);
		
		TargetArmor->TakeDurabilityDamage(DamageAmount);
	}else
	{
		if (bIsHeadHit)
		{
			FinalDamage *= 1.5f; 
		}
	}
	
	CurrentHealth = FMath::Clamp(CurrentHealth - FinalDamage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	
	if (CurrentHealth <= 0.0f)
	{
		OnZeroHealth.Broadcast();
	}
}

void UStatComponent::EquipArmor(bool bIsHead,float ReductionRate, float Durability)
{
	FArmorData* TargetArmor = bIsHead ? &HeadArmor : &BodyArmor;

	TargetArmor->ReductionRate = ReductionRate;
	TargetArmor->MaxDurability = Durability;
	TargetArmor->CurrentDurability = Durability;
}

bool UStatComponent::UseStamina(float Cost)
{
	if (!bEnableStamina) return false;
	if (CurrentStamina >= Cost)
	{
		CurrentStamina -= Cost;
		
		LastStaminaUseTime = GetWorld()->GetTimeSeconds();
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
		return true;
	}else if (CurrentStamina< Cost)
	{
		return false;
	}
	
	if (CurrentStamina > 0.0f)
	{
		CurrentStamina = 0.0f;
		LastStaminaUseTime = GetWorld()->GetTimeSeconds();
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	}
	return false;
	
}

void UStatComponent::AddAcquiredExp(float Amount)
{
	AcquiredExp += Amount;
}

void UStatComponent::ResolveExp(bool bIsEscaped)
{
	
	float FinalExp = 0.0f;
	
	if (bIsEscaped)
	{
		FinalExp = AcquiredExp;
	}else
	{
		FinalExp = AcquiredExp * 0.1f;
	}
	AcquiredExp = 0.0f;
	ProcessLevelUp(FinalExp);
}

void UStatComponent::ProcessLevelUp(float FinalAmount)
{
	if (FinalAmount <= 0.0f) return;
	
	CurrentExp += FinalAmount;
	
	while ( CurrentExp >= NextExp )
	{
		CurrentExp -= NextExp;
		CurrentLevel++;
		
		NextExp = CurrentLevel * 100.0f;
		MaxHealth += 10.0f;
		MaxStamina += 5.0f;
	}
	OnExpChanged.Broadcast(CurrentExp, NextExp);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

