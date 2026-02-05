// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

#include "Component/Utils/StatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	StatComponent->bEnableStamina = false;
	
	// 메쉬설정 이상하면 제거 아니면 bp마다 설정
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (StatComponent)
	{
		StatComponent->OnZeroHealth.AddDynamic(this,&AEnemyBase::OnEnemyZeroHealth );
	}
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEnemyBase::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	if (bIsDead) return 0.0f;
	
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator,
	                         DamageCauser);
	
	FName HitBoneName = NAME_None;
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		HitBoneName = PointDamageEvent->HitInfo.BoneName;
		if (HitBoneName.ToString().Contains(TEXT("head"), ESearchCase::IgnoreCase))
		{
			ActualDamage *=1.5f;
		}
	}
	
	if (StatComponent)
	{
		StatComponent->ApplyDamage(ActualDamage, HitBoneName);
	}

	return ActualDamage;
	
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::Die()
{
	if (bIsDead) return;
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	
	SetLifeSpan(5.0f);
}

void AEnemyBase::OnEnemyZeroHealth()
{
	Die();
}

