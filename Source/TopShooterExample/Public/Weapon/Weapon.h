// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponType: uint8
{
	Unknown UMETA(DisplayName = "Unknown"),
	Rifle UMETA(DisplayName = "Rifle"),
	Pistol UMETA(DisplayName="Pistol"),
	Knife UMETA(DisplayName="Knife"),
	Max UMETA(Hidden)
};

UCLASS()
class TOPSHOOTEREXAMPLE_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UStaticMeshComponent* WeaponMesh;
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties")
	EWeaponType WeaponType = EWeaponType::Unknown;
	
	void PlayAttackAnimation();
	
	virtual void Attack();
	
	virtual void AIAttack(AActor* TargetActor);
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* AttackMontage;
};
