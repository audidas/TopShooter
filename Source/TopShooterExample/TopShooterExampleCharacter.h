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
class  USpotLightComponent;
class UPointLightComponent;

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
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* RollAction;

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
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	
public:
	// 무기 관련
	// ----------------------------------------------
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
	// ----------------------------------------------
	
	// 이동관련
	// ----------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Move")
	float DefaultMoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Move")
	float ReloadMoveSpeed = 150.0f;
	
	UPROPERTY(EditAnywhere, Category = "Move")
	float SprintMoveSpeed = 600.0f;
	
	UPROPERTY(EditAnywhere, Category = "Move")
	float AimMoveSpeed = 200.0f;
	
	UPROPERTY(EditAnywhere, Category = "Move")
	float RollStamina = 20.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Move")
	UAnimMontage* RollMontage;
	// ----------------------------------------------
	
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDClass;
	
	UPROPERTY()
	UUserWidget* HUDWidget;
	
	// 시야용 조명
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vision System")
	USpotLightComponent* FlashLight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vision System")
	UPointLightComponent* SurroundLight;

protected:
	
	// 컨트롤러
	// -------------------------------------
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class ATopDownPlayerController* TopDownController;
	
	// -------------------------------------
	
	// 플레이어 스탯
	UPROPERTY(EditDefaultsOnly)
	class UStatComponent* StatComponent;
	
	// 장전 & 조준 
	// --------------------------------------
	bool bIsReloading= false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	bool bIsAiming = false;
	
	FTimerHandle ReloadTimerHandle;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void BP_OnReloadStart(float Duration);
	
	void StartReload();
	
	void FinishReload();
	
	void CancelReload();
	// --------------------------------------
	
	// 이동관련
	
	// --------------------------------------
	
	FRotator CachedRollRotation;
	
	bool bIsSprinting = false;
	
	float SprintCostPerSec = 15.0f; 
	
	bool bIsRolling = false;
	
	float DefaultGroundFriction;
	
	float DefaultBrakingDeceleration;

	void ToggleSprint();
	
	void Roll();
	
	UFUNCTION(BlueprintCallable)
	void AnimNotify_RollImpulse();
	
	UFUNCTION(BlueprintCallable)
	void OnRollMontageEnded();
	// --------------------------------------
public:
	// 장전 & 조준 
	// --------------------------------------
	UFUNCTION(BlueprintCallable)
	bool IsReloading() const {return bIsReloading;}
	
	UFUNCTION(BlueprintCallable)
	bool IsAiming() const { return bIsAiming; }
	
	void StartAim();
	
	void StopAim();
	// --------------------------------------
private:
	// 건물 시야 충돌 
	// --------------------------------------
	UPROPERTY()
	TArray<AActor*> OccludedActors;
	
	TMap<FName , TArray<AActor*>> BuildingGroupCache;
	
	void CheckOcclusion();
	
	// --------------------------------------
	
};

