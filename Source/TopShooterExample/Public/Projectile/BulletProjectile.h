// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TOPSHOOTEREXAMPLE_API ABulletProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABulletProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	UPROPERTY(VisibleDefaultsOnly , Category="BulletProjectile")
	USphereComponent* CollisionSphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(VisibleDefaultsOnly , Category = "BulletProjectile")
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* ImpactFX;

public:
	float Damage = 0.0f;
	
protected:
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
};
