// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AGun.h"

#include "Gameframework/TopDownPlayerController.h"
#include "Projectile/BulletProjectile.h"

// Sets default values
AAGun::AAGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAGun::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmo = MaxAmmo;
}

// Called every frame
void AAGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAGun::Attack()
{
	Super::Attack();
	
	if (!ProjectileClass) return;
	
	if (CurrentAmmo <=0 )
	{
		UE_LOG(LogTemp , Warning , TEXT("탄약 없음 : %d" ), CurrentAmmo );
		return;
	}

	FVector MuzzleLocation = WeaponMesh->DoesSocketExist(TEXT("MuzzleSocket"))
		                         ? WeaponMesh->GetSocketLocation("MuzzleSocket")
		                         : GetActorLocation();
	
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return;
	
	ATopDownPlayerController* PC = Cast<ATopDownPlayerController>(Pawn->GetController());
	if (PC)
	{
		FVector MouseLocation = PC ->GetCachedTargetLocation();
		FVector FireDirection = MouseLocation - MuzzleLocation;
		
		FireDirection.Z = 0.0f;
		FRotator MuzzleRotation = FireDirection.Rotation();
		
		// 추후 탄퍼짐 
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		
		GetWorld()->SpawnActor<ABulletProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation,SpawnParams);
		UE_LOG(LogTemp, Log, TEXT("남은 탄약: %d"), CurrentAmmo - 1);
		CurrentAmmo--;
	}
	
}

