// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/VisionComponent.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetMaterialLibrary.h"


// Sets default values for this component's properties
UVisionComponent::UVisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	VisionMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("VisionMesh"));
	VisionMesh->bUseAsyncCooking = true;
	VisionMesh->SetCastShadow(false); 
	VisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// ...
	SurroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SurroundMesh"));
	SurroundMesh->SetCastShadow(false);
	SurroundMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SurroundMesh->SetVisibleInSceneCaptureOnly(true); 
	
	FogCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("FogCapture"));

	FogCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	FogCapture->OrthoWidth = 4000.0f;
	

	FogCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	FogCapture->CompositeMode =ESceneCaptureCompositeMode::SCCM_Overwrite;
}


// Called when the game starts
void UVisionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor* Owner = GetOwner();
	if (Owner)
	{
		
		VisionMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		VisionMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
		FogCapture->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		FogCapture->SetRelativeLocation(FVector(0.0f, 0.0f, 2000.0f));
		VisionMesh->SetVisibleInSceneCaptureOnly(true);
		FogCapture->SetAbsolute(false,true,false);
		
		FogCapture->SetWorldRotation(FRotator(-90.0f, 0.0f, -90.0f));
		FogCapture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
		FogCapture->ShowOnlyComponents.Add(VisionMesh);
		
		SurroundMesh->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		SurroundMesh->SetRelativeScale3D(FVector(5.0f, 5.0f, 0.1f));
		FogCapture->ShowOnlyComponents.Add(SurroundMesh);
		if (FogRenderTarget)
		{
			FogCapture->TextureTarget = FogRenderTarget;
		}
	}
	
}


// Called every frame
void UVisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction*
                                     ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	PerformVisionScan();
	UpdateVisionFogManager();
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
	
	TArray<FVector> ViewPoints;
	ViewPoints.Add(FVector::ZeroVector);
	
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
		
		FVector HitPoint = bHit ? HitResult.Location : EndPos;
		FVector LocalPoint = Owner->GetTransform().InverseTransformPosition(HitPoint);
		LocalPoint.Z = 10.0f; 

		ViewPoints.Add(LocalPoint);
	}
	UpdateVisionMesh(ViewPoints);
}

void UVisionComponent::UpdateVisionMesh(const TArray<FVector>& ViewPoints)
{
	if (ViewPoints.Num() < 3) return;

	TArray<FVector> Vertices = ViewPoints;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;
	
	for (int32 i = 1; i < Vertices.Num() - 1; i++)
	{
		Triangles.Add(0);
		Triangles.Add(i+1);
		Triangles.Add(i);
	}
	
	VisionMesh->CreateMeshSection_LinearColor(
		0, 
		Vertices, 
		Triangles, 
		Normals, 
		UV0, 
		Colors, 
		Tangents, 
		false 
	);
	
	if (VisionMaterial)
	{
		VisionMesh->SetMaterial(0, VisionMaterial);
	}
}

void UVisionComponent::UpdateVisionFogManager()
{
	AActor* Owner = GetOwner();
	if (Owner && FogMPC)
	{
		FVector PlayerPos = Owner->GetActorLocation();
		UKismetMaterialLibrary::SetVectorParameterValue(this, FogMPC, FName("PlayerLocation"), FLinearColor(PlayerPos));
		
		UKismetMaterialLibrary::SetScalarParameterValue(this, FogMPC, FName("OrthoWidth"), FogCapture->OrthoWidth);
	}
}

