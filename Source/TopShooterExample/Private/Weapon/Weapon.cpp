// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

#include "GameFramework/Character.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	
	WeaponMesh ->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::PlayAttackAnimation()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	if (OwnerCharacter && AttackMontage)
	{	
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			OwnerCharacter->PlayAnimMontage(AttackMontage);
		}
	}
}

void AWeapon::Attack()
{
PlayAttackAnimation();
}

void AWeapon::AIAttack(AActor* TargetActor)
{
	
PlayAttackAnimation();
	
}

