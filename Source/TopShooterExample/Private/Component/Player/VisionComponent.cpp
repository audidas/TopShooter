// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/VisionComponent.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/OverlapResult.h"
#include "Kismet/KismetMaterialLibrary.h"


// Sets default values for this component's properties
UVisionComponent::UVisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	
}


// Called when the game starts
void UVisionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor* Owner = GetOwner();
	
}


// Called every frame
void UVisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction*
                                     ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	PerformVisionScan();

	// ...
}

void UVisionComponent::PerformVisionScan()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;
	
	FVector StartPos = Owner -> GetActorLocation();
	FVector ForwardVector = Owner-> GetActorForwardVector();
	float HalfFOV = ViewAngleFOV * 0.5f;
	float AngleStep = ViewAngleFOV / static_cast<float>(TraceResolution);
	
	TSet<AActor*> CurrentVisibleActors;
	
	float SurroundRadius = 350.0f;
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(SurroundRadius);
	
	bool bOverlap = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		StartPos,
		FQuat::Identity,
		ECC_Visibility, 
		CollisionShape,
		FCollisionQueryParams(NAME_None, false, Owner)
	);
	
	if (bOverlap)
	{
		for (const FOverlapResult Result : OverlapResults)
		{
			AActor* OverlapActor = Result.GetActor();
			if (OverlapActor && OverlapActor->ActorHasTag(TEXT("Enemy")))
			{
				CurrentVisibleActors.Add(OverlapActor);
				OverlapActor->SetActorHiddenInGame(false);
			}
		}
	}
	
	for (int32 i =0 ; i<=TraceResolution; i++)
	{
		float CurrentAngleDeg = -HalfFOV + (AngleStep * i);
		FVector Direction = ForwardVector.RotateAngleAxis(CurrentAngleDeg, FVector::UpVector);
		FVector EndPos = StartPos + (Direction * ViewRadius);
		
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Owner);
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartPos,
			EndPos,
			ECC_Visibility,
			QueryParams
		);
		if (bHit && HitResult.GetActor())
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor->ActorHasTag(TEXT("Enemy")))
			{
				CurrentVisibleActors.Add(HitActor);
				HitActor->SetActorHiddenInGame(false);
			}
		}
		
	}
	
	for (AActor* OldActor : LastVisibleActors)
	{
		if (IsValid(OldActor) && !CurrentVisibleActors.Contains(OldActor))
		{
			OldActor->SetActorHiddenInGame(true);
		}
	}
	LastVisibleActors = CurrentVisibleActors;
}


