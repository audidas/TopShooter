// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameframework/TopDownPlayerCameraManager.h"

void ATopDownPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT,
	float DeltaTime)
{
	Super::UpdateViewTargetInternal(OutVT, DeltaTime);
	
	FVector TargetLocation = OutVT.Target->GetActorLocation();
	OutVT.POV.Location = TargetLocation + FixedCameraOffset;
	FVector LookVector = TargetLocation - OutVT.POV.Location;
	OutVT.POV.Rotation = LookVector.Rotation();
	OutVT.POV.FOV = FixedFOV;
}
