// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "TopDownPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class TOPSHOOTEREXAMPLE_API
	ATopDownPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	FVector FixedCameraOffset = FVector(1000.f, 0.f, 1000.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float FixedFOV = 90.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float AimZoomFactor = 0.9f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float ZoomInterpSpeed = 10.0f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = "Camera|Pan")
	float MaxPanDistance = 300.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Pan")
	float PanInterpSpeed = 5.0f;
	
protected:
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;
	
private:
	FVector CurrentCameraOffset;
	
	FVector CurrentPanOffset = FVector::ZeroVector;
	
	bool bFirstFrame= true;
};
