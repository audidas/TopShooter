// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AGun.h"

#include "Gameframework/TopDownPlayerController.h"
#include "Projectile/BulletProjectile.h"
#include "Kismet/GameplayStatics.h"

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

		ABulletProjectile* Bullet = GetWorld()->SpawnActor<ABulletProjectile>(
			ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		
		if (Bullet)
		{
			Bullet->Damage = this->Damage;
		}
		CurrentAmmo--;
	}
	
	if (MuzzleFlashFX)
	{
		UGameplayStatics::SpawnEmitterAttached(
			MuzzleFlashFX,
			WeaponMesh,
			TEXT("MuzzleSocket"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::Type::SnapToTarget,
			true
		);
	}
	
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			FireSound,
			GetActorLocation()
			);
	}
	
	if (FireCameraShake)
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		
		if (PC)
		{
			PC->ClientStartCameraShake(FireCameraShake,1.0f);
		}
	}
}

void AAGun::PlayReloadSound()
{
	if (ReloadSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
		this,
		ReloadSound,
		GetActorLocation()
		);
	}
	
}

void AAGun::Reload()
{

	CurrentAmmo = MaxAmmo;
}

