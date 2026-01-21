// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TartgetDummy.generated.h"

UCLASS()
class TOPSHOOTEREXAMPLE_API ATartgetDummy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATartgetDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	class UStaticMeshComponent* MeshComp;
	
};
