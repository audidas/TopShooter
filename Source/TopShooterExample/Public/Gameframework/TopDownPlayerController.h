// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class TOPSHOOTEREXAMPLE_API ATopDownPlayerController : public APlayerController
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
	
	
};
