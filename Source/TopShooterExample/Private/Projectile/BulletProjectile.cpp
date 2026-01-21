// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/BulletProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABulletProjectile::ABulletProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionSphereComponent->InitSphereRadius(5.f);
	CollisionSphereComponent->SetCollisionProfileName(TEXT("Bullet"));

	RootComponent = CollisionSphereComponent;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetCollisionProfileName(TEXT("NoCollision"));
	ProjectileMesh->SetupAttachment(RootComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionSphereComponent;
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
	
}

// Called when the game starts or when spawned
void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionSphereComponent->OnComponentHit.AddDynamic(this , &ABulletProjectile::OnHit);
}

// Called every frame
void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABulletProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		UGameplayStatics::ApplyPointDamage(
		OtherActor,
		Damage,
		GetActorForwardVector(),
		Hit,
		GetInstigatorController(),
		this,
		UDamageType::StaticClass()
		);
		
		if (ImpactFX)
		{
			FRotator EffectRotation = Hit.ImpactNormal.Rotation();
			UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ImpactFX,
			Hit.ImpactPoint,
			EffectRotation,
			true
			);
		}
		Destroy();
	}
	
	
}

