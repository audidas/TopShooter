// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "TopDownPlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class TOPSHOOTEREXAMPLE_API ATopDownPlayerController : public APlayerController , public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ATopDownPlayerController();
	
	virtual void Tick( float DeltaTime ) override;

protected:
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	
private: 
	
	FVector TargetLocation;
	
	FGenericTeamId TeamId;
	
public :
	
	FVector GetCachedTargetLocation() const {return TargetLocation;};
	
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	

	
};
