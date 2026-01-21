// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "AGun.generated.h"

UCLASS()
class TOPSHOOTEREXAMPLE_API AAGun : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(EditDefaultsOnly , Category="Combat")
	TSubclassOf<class ABulletProjectile> ProjectileClass;
	
protected:
	
	virtual void Attack() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	int32 MaxAmmo =30;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 CurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere , Category="Combat")
	class UParticleSystem* MuzzleFlashFX;

public:
	int32 GetCurrentAmmo() const {return CurrentAmmo;};
	int32 GetMaxAmmo() const {return MaxAmmo;};
	
	void Reload();
	
};
