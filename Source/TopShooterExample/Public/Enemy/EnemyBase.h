// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class TOPSHOOTEREXAMPLE_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;
	
	
public:
	// 무기
	// --------------------------------------
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AWeapon> StartingWeaponClass;
	
	void SpawnDefaultWeapon();
	void FireWeapon(AActor* Target);
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	class AWeapon* CurrentWeapon;
	// --------------------------------------
	
	// Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthWidgetComp;
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStatComponent* StatComponent;
	
	virtual void Die();
	
	UFUNCTION()
	void OnEnemyZeroHealth();
	
	bool bIsDead = false;
};
