// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameframework/TopDownPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

ATopDownPlayerController::ATopDownPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATopDownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	
	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem-> AddMappingContext(CurrentContext , 0);
			}
		}
	}
}

void ATopDownPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float MouseX , MouseY;
	
	if (GetMousePosition(MouseX, MouseY))
	{
		FVector WorldLocation , WorldDirection;
		
		if (DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
		{
			FHitResult HitResult;
			FVector TraceEnd = WorldLocation +(WorldDirection * 10000.0f);
			
			FCollisionQueryParams QueryParameters;
			QueryParameters.AddIgnoredActor(GetPawn());
			
			bool bHit = GetWorld()-> LineTraceSingleByChannel(
				HitResult,
				WorldLocation,
				TraceEnd,
				ECC_Visibility,
				QueryParameters
			);
			
			if (bHit)
			{
				FVector TargetLocation = HitResult.Location;
				
				if (APawn* ControllerPawn = GetPawn())
				{
					FVector PawnLocation  =ControllerPawn->GetActorLocation();
					
					FVector LookVector = TargetLocation - PawnLocation;
					LookVector.Z =0.0f;
					
					FRotator LookRotation = LookVector.Rotation();
					SetControlRotation(LookRotation);
				}
			}
		}
	}
	
}