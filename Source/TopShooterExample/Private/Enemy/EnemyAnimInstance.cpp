// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnimInstance.h"

#include "Enemy/EnemyBase.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Weapon.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (!GetWorld()) return;
    
	OwnerCharacter = Cast<AEnemyBase>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
	
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			CachedTarget = PC->GetPawn();
		}
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	

	
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<AEnemyBase>(TryGetPawnOwner());
	}
	if (!OwnerCharacter) return;
	
	if (CurrentWeaponType == EWeaponType::Unknown)
	{
		
	if (OwnerCharacter->CurrentWeapon)
	{
		CurrentWeaponType = OwnerCharacter->CurrentWeapon->WeaponType;
	}
	}
	
	FVector Velocity = OwnerCharacter->GetVelocity();
	Velocity.Z = 0;
	GroundSpeed = Velocity.Size();
	
	bShouldMove = GroundSpeed > 3.0f;
	if (!CachedTarget) 
	{
		if (UWorld* World = GetWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				CachedTarget = PC->GetPawn();
			}
		}
	}
	
	
	if (CachedTarget)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
				OwnerCharacter->GetActorLocation(), 
				CachedTarget->GetActorLocation()
			);
		
		FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(
			LookAtRotation, 
			OwnerCharacter->GetActorRotation()
		);
		
		float ClampedYaw = FMath::Clamp(DeltaRot.Yaw, -45.0f, 45.0f);
		
		if (IsAnyMontagePlaying())
		{
			ClampedYaw=45.0f;
		}
		
		float SmoothYaw = FMath::FInterpTo(SpineRotation.Yaw, ClampedYaw, DeltaSeconds, 10.0f);
        
		SpineRotation = FRotator(0.0f, SmoothYaw, 0.0f);
	}
}
