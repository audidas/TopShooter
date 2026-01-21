// Fill out your copyright notice in the Description page of Project Settings.


#include "TartgetDummy.h"


// Sets default values
ATartgetDummy::ATartgetDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComp =CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent =MeshComp;
	
	MeshComp->SetCollisionProfileName(TEXT("Pawn"));
}

// Called when the game starts or when spawned
void ATartgetDummy::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth=MaxHealth;
}

// Called every frame
void ATartgetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATartgetDummy::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator,
	                         DamageCauser);
	
    if (ActualDamage > 0.0f)
    {
	    CurrentHealth -= ActualDamage;
    	
    	UE_LOG(LogTemp, Warning, TEXT("Dummy Hit! Damage: %f, Health Left: %f"), ActualDamage, CurrentHealth);
    	
    	if (CurrentHealth <= 0.0f)
    	{
    		UE_LOG(LogTemp, Error, TEXT("Dummy Destroyed!"));
    		Destroy();
    	}
    }
	return ActualDamage;
}

