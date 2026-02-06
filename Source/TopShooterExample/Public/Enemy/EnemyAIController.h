// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class TOPSHOOTEREXAMPLE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void OnUnPossess() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY()
	class APawn* PlayerPawn;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 800.0f;
	
public:
	// 팀 시스템
	// ---------------------------------------------
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	// ---------------------------------------------
	
private:
	FGenericTeamId TeamId;
};
