// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameframework/TopDownPlayerCameraManager.h"

#include "TopShooterExampleCharacter.h"
#include "Gameframework/TopDownPlayerController.h"

void ATopDownPlayerCameraManager::AddRecoil(FVector Direction, float Strength)
{
	
	Direction.Normalize();
	
	Direction.Z = 0.0f;
	
	CurrentRecoilOffset += Direction * Strength;
}

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
			
			float CenterX = SizeX *0.5f;
			float CenterY = SizeY *0.5f;
            
			float RatioX = (MouseX - CenterX) / CenterX;
			float RatioY = (MouseY - CenterY) / CenterY;
			
			RatioX = FMath::Clamp(RatioX, -1.0f, 1.0f);
			RatioY = FMath::Clamp(RatioY, -1.0f, 1.0f);
			
			TargetPanOffset.X = -RatioY * MaxPanDistance; 
			TargetPanOffset.Y = RatioX * MaxPanDistance;
		}
	}
	
	CurrentPanOffset = FMath::VInterpTo(CurrentPanOffset, TargetPanOffset, DeltaTime, PanInterpSpeed);
	
	if (!CurrentRecoilOffset.IsZero())
	{
		CurrentRecoilOffset = FMath::VInterpTo(CurrentRecoilOffset, FVector::ZeroVector, DeltaTime, RecoilRecoverySpeed);
	}
	
	FVector CharacterLocation  = OutVT.Target->GetActorLocation();
	FVector FinalTargetLocation = CharacterLocation + CurrentPanOffset;
	OutVT.POV.Location = FinalTargetLocation + CurrentCameraOffset + CurrentRecoilOffset;
	FVector LookVector = FinalTargetLocation - OutVT.POV.Location;
	
	OutVT.POV.Rotation = LookVector.Rotation();
	OutVT.POV.FOV = FixedFOV;

}
