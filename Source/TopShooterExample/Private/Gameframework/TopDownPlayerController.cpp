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
	
	bShowMouseCursor = true;
	
	TeamId = FGenericTeamId(0);
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

void ATopDownPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	IGenericTeamAgentInterface::SetGenericTeamId(NewTeamID);
	TeamId = NewTeamID;
}

FGenericTeamId ATopDownPlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void ATopDownPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float MouseX , MouseY;
	APawn* MyPawn = GetPawn();
	if (!MyPawn) return;
	if (GetMousePosition(MouseX, MouseY))
	{
		FVector WorldLocation , WorldDirection;
		
		if (!IsLookInputIgnored())
		{
			
		
		if (DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
			{
			float GunHeight = MyPawn->GetActorLocation().Z + 85.0f;
			if (FMath::IsNearlyZero(WorldDirection.Z)) return;
			
			float T = (GunHeight - WorldLocation.Z) / WorldDirection.Z;
			FVector IntersectionPoint = WorldLocation + ( WorldDirection * T);
			TargetLocation = IntersectionPoint;
			}
		}
	}
}
