// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameframework/TopDownPlayerCameraManager.h"

#include "TopShooterExampleCharacter.h"
#include "Gameframework/TopDownPlayerController.h"

void ATopDownPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT,
                                                           float DeltaTime)
{
	Super::UpdateViewTargetInternal(OutVT, DeltaTime);
	
	if (!OutVT.Target) return;
	
	if (bFirstFrame)
	{
		CurrentCameraOffset = FixedCameraOffset;
		bFirstFrame = false;
	}
	FVector TargetOffset = FixedCameraOffset;
	ATopShooterExampleCharacter* MyChar = Cast<ATopShooterExampleCharacter>(OutVT.Target);
	if (MyChar && MyChar->IsAiming())
	{
		TargetOffset = FixedCameraOffset * AimZoomFactor;
	}
	
	CurrentCameraOffset = FMath::VInterpTo(
		CurrentCameraOffset, 
		TargetOffset, 
		DeltaTime, 
		ZoomInterpSpeed
	);
	
	FVector TargetPanOffset = FVector::ZeroVector;
	if (ATopDownPlayerController* PC  = Cast<ATopDownPlayerController>(GetOwningPlayerController()))
	{
		float MouseX, MouseY;
		int32 SizeX, SizeY;
		
		if (PC ->GetMousePosition(MouseX, MouseY))
		{
			PC->GetViewportSize(SizeX, SizeY);
			
			float CenterX = SizeX / 2.0f;
			float CenterY = SizeY / 2.0f;
            
			float RatioX = (MouseX - CenterX) / CenterX;
			float RatioY = (MouseY - CenterY) / CenterY;
			
			RatioX = FMath::Clamp(RatioX, -1.0f, 1.0f);
			RatioY = FMath::Clamp(RatioY, -1.0f, 1.0f);
			
			TargetPanOffset.X = RatioY * MaxPanDistance; 
			TargetPanOffset.Y = -RatioX * MaxPanDistance;
		}
	}
	
	CurrentPanOffset = FMath::VInterpTo(CurrentPanOffset, TargetPanOffset, DeltaTime, PanInterpSpeed);
	
	FVector CharacterLocation  = OutVT.Target->GetActorLocation();
	FVector FinalTargetLocation = CharacterLocation + CurrentPanOffset;
	OutVT.POV.Location = FinalTargetLocation + CurrentCameraOffset;
	FVector LookVector = FinalTargetLocation - OutVT.POV.Location;
	
	OutVT.POV.Rotation = LookVector.Rotation();
	OutVT.POV.FOV = FixedFOV;

}
