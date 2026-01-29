// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/VisionComponent.h"


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
			ECC_Visibility, // 시야를 가리는 채널 (필요시 커스텀 채널로 변경)
			QueryParams
		);
		
		if (bShowDebugLines)
		{
			if (bHit)
			{
				DrawDebugLine(GetWorld(), StartPos, HitResult.Location, FColor::Red, false, -1.0f, 0, 1.0f);
				DrawDebugPoint(GetWorld(), HitResult.Location, 5.0f, FColor::Red, false, -1.0f);
			}else
			{
				DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Green, false, -1.0f, 0, 1.0f);
			}
		}
	}
}

