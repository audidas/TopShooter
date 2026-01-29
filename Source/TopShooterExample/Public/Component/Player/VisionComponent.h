// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "VisionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPSHOOTEREXAMPLE_API UVisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVisionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;
	
	
private: 
	void PerformVisionScan();
	
	void UpdateVisionMesh(const TArray<FVector>& ViewPoints);
	
public:
	UPROPERTY(EditAnywhere, Category = "Vision")
	float ViewRadius = 800.0f;
	
	UPROPERTY(EditAnywhere, Category = "Vision")
	float ViewAngleFOV = 90.0f;
	
	UPROPERTY(EditAnywhere, Category = "Vision")
	int32 TraceResolution = 50;
	
	UPROPERTY(EditAnywhere, Category = "Vision")
	bool bShowDebugLines = true;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly , Category="Vision")
	UProceduralMeshComponent* VisionMesh;
	
	UPROPERTY(EditAnywhere, Category = "Vision")
	UMaterialInterface* VisionMaterial;
};
