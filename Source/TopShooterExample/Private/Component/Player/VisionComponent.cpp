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

	FVector StartPos = Owner->GetActorLocation();
	FVector ForwardVector = Owner->GetActorForwardVector();
	
	TSet<AActor*> CurrentVisibleActors;
	
	float SurroundRadius = 350.0f;
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(ViewRadius);
	
	FCollisionQueryParams QueryParams(NAME_None, false, Owner);
	bool bOverlap = GetWorld()->OverlapMultiByChannel(
			OverlapResults,
			StartPos,
			FQuat::Identity,
			ECC_Pawn, 
			CollisionShape,
			QueryParams
		);
	
if (bOverlap)
    {
        for (const FOverlapResult& Result : OverlapResults)
        {
            AActor* TargetActor = Result.GetActor();
        	
            if (!TargetActor || !TargetActor->ActorHasTag(TEXT("Enemy"))) continue;

            FVector TargetPos = TargetActor->GetActorLocation();
            FVector DirToTarget = (TargetPos - StartPos);
            float DistanceToTarget = DirToTarget.Size();
            DirToTarget.Normalize();

            bool bIsVisible = false;
        	
            if (DistanceToTarget <= SurroundRadius)
            {
                bIsVisible = true;
            }
           
            else 
            {
                float DotProduct = FVector::DotProduct(ForwardVector, DirToTarget);
                float AngleThreshold = FMath::Cos(FMath::DegreesToRadians(ViewAngleFOV * 0.5f));

                if (DotProduct >= AngleThreshold)
                {
                    FHitResult WallHit;
                    bool bHitWall = GetWorld()->LineTraceSingleByChannel(
                        WallHit,
                        StartPos,
                        TargetPos, 
                        ECC_Visibility,
                        QueryParams
                    );

  
                    if (!bHitWall || WallHit.GetActor() == TargetActor)
                    {
                        bIsVisible = true;
                    }
                }
            	
            }
        	
            if (bIsVisible)
            {
                CurrentVisibleActors.Add(TargetActor);
                TargetActor->SetActorHiddenInGame(false);
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


