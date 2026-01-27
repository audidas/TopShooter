// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TopShooterExampleCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

class AWeapon;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ATopShooterExampleCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AimAction;

public:

	/** Constructor */
	ATopShooterExampleCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	void Attack();

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABulletProjectile> BulletProjectileClass;
	
	UPROPERTY(EditDefaultsOnly , Category="Combat")
	TSubclassOf<AWeapon> DefaultWeaponClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	AWeapon* CurrentWeapon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* FireMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* ReloadMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDClass;
	
	UPROPERTY()
	UUserWidget* HUDWidget;
	

protected:
	bool bIsReloading= false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	bool bIsAiming = false;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void BP_OnReloadStart(float Duration);
	
	void StartReload();
	
	void FinishReload();
	
public:
	UFUNCTION(BlueprintCallable)
	bool IsReloading() const {return bIsReloading;}
	
	UFUNCTION(BlueprintCallable)
	bool IsAiming() const { return bIsAiming; }
	
	void StartAim();
	
	void StopAim();
	
private:
	UPROPERTY()
	TArray<AActor*> OccludedActors;
	
	TMap<FName , TArray<AActor*>> BuildingGroupCache;
	
	void CheckOcclusion();
	
};

