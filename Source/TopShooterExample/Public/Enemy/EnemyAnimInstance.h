// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

enum class EWeaponType : uint8;
/**
 * 
 */
UCLASS()
class TOPSHOOTEREXAMPLE_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bShouldMove;

	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsAttacking;
	
	UPROPERTY()
	class AEnemyBase* OwnerCharacter;
	
	UPROPERTY(BlueprintReadOnly)
	AActor* CachedTarget;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	EWeaponType CurrentWeaponType;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FRotator SpineRotation;
};
