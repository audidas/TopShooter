// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"

#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"


// Sets default values
AEnemyAIController::AEnemyAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TeamId = FGenericTeamId(1);
}

// Called when the game starts or when spawned
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	PlayerPawn = nullptr;
}

// Called every frame
void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AEnemyBase* MyEnemy = Cast<AEnemyBase>(GetPawn());
	

	if (PlayerPawn == nullptr)
	{
		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		
		if (PlayerPawn == nullptr) return;
	}
	
	if (!MyEnemy|| !PlayerPawn) return;
	
	float DistanceToPlayer = FVector::Dist(MyEnemy->GetActorLocation(), PlayerPawn->GetActorLocation());
	
	if (DistanceToPlayer > AttackRange)
	{
		MoveToActor(PlayerPawn,  AttackRange -100.0f);

		ClearFocus(EAIFocusPriority::Gameplay);
	}else
	{
		StopMovement();
		
		SetFocus(PlayerPawn);
		
		MyEnemy->FireWeapon(PlayerPawn);
	}
}

void AEnemyAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	Super::SetGenericTeamId(NewTeamID);
	
	TeamId = NewTeamID;
}

FGenericTeamId AEnemyAIController::GetGenericTeamId() const
{
	return TeamId;
}

ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(
	const AActor& Other) const
{
	const APawn* OtherPawn = Cast<const APawn>(&Other);
	if (!OtherPawn) return ETeamAttitude::Neutral;
	
	auto TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (!TeamAgent) return ETeamAttitude::Neutral;
	
	FGenericTeamId OtherTeamId = TeamAgent->GetGenericTeamId();
	
	if (OtherTeamId == TeamId)
	{
		return ETeamAttitude::Friendly;
	}
	
	return ETeamAttitude::Hostile;
}

