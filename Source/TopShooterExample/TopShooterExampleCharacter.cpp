// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopShooterExampleCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TopShooterExample.h"
#include "Blueprint/UserWidget.h"
#include "Component/Utils/StatComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/DamageEvents.h"
#include "Gameframework/TopDownPlayerController.h"
#include "Interface/OCFadeInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/AGun.h"
#include "Weapon/Weapon.h"

ATopShooterExampleCharacter::ATopShooterExampleCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	// 캐릭터 스탯
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	
	// 캐릭터 시야용 조명
	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(GetMesh());
	FlashLight->SetRelativeLocationAndRotation(FVector(-30.f, 70.f, 140.f) , FRotator(0.f, 70.0f, 0.0f));
	
	// 캐릭터 앞 시야 조명
	FlashLight->Intensity = 10.0f;
	FlashLight->AttenuationRadius = 3500.0f;
	FlashLight->OuterConeAngle = 45.0f;
	FlashLight->InnerConeAngle = 45.0f;
	FlashLight->bUseInverseSquaredFalloff = false;
	FlashLight->LightFalloffExponent = 1.0f;
	
	FlashLight->SourceRadius =0.0f;
	FlashLight->SoftSourceRadius=0.0f;
	FlashLight->CastShadows = true;
	
	
	
	// 캐릭터 주변 조명
	SurroundLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("SurroundLight"));
	SurroundLight->SetupAttachment(RootComponent);
	SurroundLight->SetRelativeLocation(FVector(0.f, 0.f, 140.f));
	
	SurroundLight->bUseInverseSquaredFalloff=false;
	SurroundLight->Intensity = 10.0f;
	SurroundLight->LightFalloffExponent = 1.0f;
	SurroundLight->AttenuationRadius = 400.0f;
	
	SurroundLight->SourceRadius = 0.0f;
	SurroundLight->SoftSourceRadius = 0.0f;
	SurroundLight->CastShadows = true;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> LightFuncMatAsset(TEXT("/Game/Materials/Fog/M_FlashLight.M_FlashLight"));
	if (LightFuncMatAsset.Succeeded())
	{
	FlashLight-> LightFunctionMaterial = LightFuncMatAsset.Object;
	}
	
	if (GetMesh())
	{
		FLightingChannels Channels;
		
		Channels.bChannel0 = true;
		Channels.bChannel1 = true;
		Channels.bChannel2 = false;
		GetMesh()->SetLightingChannels(Channels.bChannel0, Channels.bChannel1, Channels.bChannel2);
		
		FLightingChannels SurroundChannels;
		SurroundChannels.bChannel0 = true;
		SurroundChannels.bChannel1 = true;
		SurroundChannels.bChannel2 = false;
		
		SurroundLight->SetLightingChannels(SurroundChannels.bChannel0, SurroundChannels.bChannel1, SurroundChannels.bChannel2);
		
		FLightingChannels FlashChannels;
		FlashChannels.bChannel0 = true;
		FlashChannels.bChannel1 = true; 
		FlashChannels.bChannel2 = false;
		FlashLight->SetLightingChannels(FlashChannels.bChannel0, FlashChannels.bChannel1, FlashChannels.bChannel2);
	}
	
}

void ATopShooterExampleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopShooterExampleCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATopShooterExampleCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATopShooterExampleCharacter::Look);
		
		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started , this , &ATopShooterExampleCharacter::Attack);
		
		// Reload
		EnhancedInputComponent->BindAction(ReloadAction ,ETriggerEvent::Started , this , &ATopShooterExampleCharacter::StartReload );
		
		// Aiming
		EnhancedInputComponent->BindAction(AimAction , ETriggerEvent::Started ,this ,&ATopShooterExampleCharacter::StartAim);
		EnhancedInputComponent->BindAction(AimAction , ETriggerEvent::Completed ,this ,&ATopShooterExampleCharacter::StopAim);
		
		// Sprint
		EnhancedInputComponent->BindAction(SprintAction , ETriggerEvent::Started , this , &ATopShooterExampleCharacter::ToggleSprint);
		
		// Roll
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started ,this , & ATopShooterExampleCharacter::Roll);
	}
	else
	{
		UE_LOG(LogTopShooterExample, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATopShooterExampleCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
	
}

void ATopShooterExampleCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATopShooterExampleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	TopDownController = Cast<ATopDownPlayerController>(GetController());
	DefaultBrakingDeceleration = GetCharacterMovement()->BrakingDecelerationWalking;
	DefaultGroundFriction = GetCharacterMovement()->GroundFriction;
	
	if (DefaultWeaponClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this;
			
			AWeapon* SpawningWeapon = World->SpawnActor<AWeapon>(DefaultWeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);
			if (SpawningWeapon)
			{
				CurrentWeapon = SpawningWeapon;
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("HandGrip_R"));
			}
		}
	}
	
	if ( StatComponent)
	{
		StatComponent->bEnableStamina = true;
		StatComponent->SetComponentTickEnabled(true);
	}
	
	if (HUDClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void ATopShooterExampleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsRolling)
	{
		SetActorRotation(CachedRollRotation);
	}else if (TopDownController)
	{
		FVector TargetPoint = TopDownController->GetCachedTargetLocation();
		TargetPoint.Z = GetActorLocation().Z;
		FVector LookVector = TargetPoint - GetActorLocation();
		FRotator LookRotation = FRotationMatrix::MakeFromX(LookVector).Rotator();
		SetActorRotation(LookRotation);
		CachedRollRotation = LookRotation;
	}
	
	CheckOcclusion();
	
	if (bIsSprinting)
	{
		float Cost = SprintCostPerSec * DeltaTime;
		bool bSuccess = StatComponent ->UseStamina(Cost);
		if (!bSuccess)
		{
			ToggleSprint();
		}
	}
	
}

void ATopShooterExampleCharacter::Attack()
{
	if (bIsReloading) return;
	if ( CurrentWeapon)
	{
		CurrentWeapon->Attack();
	}
	
	if (FireMontage)
	{
		PlayAnimMontage(FireMontage);
	}
}

void ATopShooterExampleCharacter::DoMove(float Right, float Forward)
{
	
	if (TopDownController && TopDownController->PlayerCameraManager)
	{
		// find out which way is forward
		const FRotator Rotation = TopDownController ->PlayerCameraManager->GetCameraRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
	
}

void ATopShooterExampleCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ATopShooterExampleCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ATopShooterExampleCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

float ATopShooterExampleCharacter::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator,
	                         DamageCauser);
	
	
	FName HitBoneName = NAME_None;
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		HitBoneName = PointEvent->HitInfo.BoneName;
	}
	
	if (StatComponent)
	{
		StatComponent->ApplyDamage(ActualDamage, HitBoneName);
	}
	return ActualDamage;
}

void ATopShooterExampleCharacter::StartReload()
{
	
	if (bIsReloading || ! CurrentWeapon) return;
	if (bIsAiming) StopAim();
	if (bIsSprinting) 
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed; 
	}
	if (AAGun* CurrentGun = Cast<AAGun>(CurrentWeapon))
	{
		CurrentGun->PlayReloadSound();
	}
	bIsReloading = true;
	
	GetCharacterMovement()->MaxWalkSpeed = ReloadMoveSpeed;
	
	float Duration = 2.0f;
	
	if (ReloadMontage)
	{
		Duration = PlayAnimMontage(ReloadMontage);
	}
	BP_OnReloadStart(Duration);
	
	
	GetWorldTimerManager().SetTimer(ReloadTimerHandle , this , &ATopShooterExampleCharacter::FinishReload, Duration ,false);
	
}

void ATopShooterExampleCharacter::FinishReload()
{
	
	bIsReloading = false;
	GetCharacterMovement()->MaxWalkSpeed =DefaultMoveSpeed;
	
	AAGun* CurrentGun = Cast<AAGun>(CurrentWeapon);
	if (CurrentWeapon && CurrentGun)
	{
		CurrentGun->Reload();
	}
}

void ATopShooterExampleCharacter::CancelReload()
{
	if (!bIsReloading) return;
	if (ReloadMontage)
	{
		StopAnimMontage(ReloadMontage);
	}
	
	bIsReloading = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
	
	// 나중에 UI 장전 bar 취소용
}

void ATopShooterExampleCharacter::ToggleSprint()
{

	if (bIsRolling || bIsReloading) return;
	
	if (bIsAiming)
	{
		StopAim();
	}
	bIsSprinting = !bIsSprinting;
	
	
	GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintMoveSpeed : DefaultMoveSpeed;
	
}

void ATopShooterExampleCharacter::Roll()
{
	if (bIsRolling) return;
	
	if (bIsReloading)
	{
		CancelReload();
	}
	if (bIsAiming)
	{
		StopAim();
	}
	
	if (StatComponent && !StatComponent->UseStamina(RollStamina))
	{
		return;
	}
	bIsRolling = true;
	bIsSprinting = false;
	
	if (RollMontage)
	{
			PlayAnimMontage(RollMontage);
	}
	
}

void ATopShooterExampleCharacter::AnimNotify_RollImpulse()
{
	
	FVector LaunchDir = GetActorForwardVector();
	GetCharacterMovement()->GroundFriction = 0.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.0f;
	LaunchCharacter(LaunchDir * 2500.0f, true, true);
}

void ATopShooterExampleCharacter::OnRollMontageEnded()
{
	bIsRolling = false;
	GetCharacterMovement()->GroundFriction = DefaultGroundFriction;
	GetCharacterMovement()->BrakingDecelerationWalking = DefaultBrakingDeceleration;
}

void ATopShooterExampleCharacter::StartAim()
{
	if (bIsReloading || bIsRolling) return;
	
	if (bIsSprinting)
	{
		bIsSprinting = false;
	}
	bIsAiming = true;
	GetCharacterMovement()->MaxWalkSpeed = AimMoveSpeed;
}

void ATopShooterExampleCharacter::StopAim()
{
	bIsAiming = false;
	if (!bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
	}
}

void ATopShooterExampleCharacter::CheckOcclusion()
{
	
	if (!TopDownController|| !TopDownController->PlayerCameraManager) return;
	
	
	FVector CameraLoc = TopDownController->PlayerCameraManager->GetCameraLocation();
	FVector PlayerLoc = GetActorLocation();
	
	
	TArray<FHitResult> OutHits; 
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	GetWorld()->SweepMultiByChannel(OutHits, CameraLoc, PlayerLoc, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(40.0f), Params);
	
	TArray<AActor*> ActorsToHide;
	
	for (const FHitResult& Hit : OutHits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || !HitActor->GetClass()->ImplementsInterface(UOCFadeInterface::StaticClass())) continue;
		
		FName GroupTag = NAME_None;
		for (const FName& Tag : HitActor ->Tags)
		{
			if (Tag.ToString().StartsWith("Building_"))
			{
				GroupTag = Tag;
				break;
			}
		}
		
		if ( GroupTag != NAME_None )
		{
			if (BuildingGroupCache.Contains(GroupTag))
			{
				TArray<AActor*>& CachedGroup = BuildingGroupCache[GroupTag];
				for (AActor* Member : CachedGroup)
				{
					if (IsValid(Member)) ActorsToHide.AddUnique(Member);
				}
			}else
			{
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsWithTag(GetWorld(), GroupTag, FoundActors);
				TArray<AActor*> ValidMembers;
				for (AActor* Member : FoundActors)
				{
					if (Member && Member->GetClass()->ImplementsInterface(UOCFadeInterface::StaticClass()))
					{
						ValidMembers.Add(Member);
						ActorsToHide.AddUnique(Member);
					}
				}
				BuildingGroupCache.Add(GroupTag, ValidMembers);
			}
		}else
		{
			ActorsToHide.AddUnique(HitActor);
		}
	}
	for (AActor* OldActor : OccludedActors)
	{
		if (IsValid(OldActor) && !ActorsToHide.Contains(OldActor))
		{
			IOCFadeInterface::Execute_SetFadeOpacity(OldActor, 1.0f);
		}
	}
	
	for ( AActor* NewActor : ActorsToHide)
	{
		if (IsValid(NewActor) && !OccludedActors.Contains(NewActor))
		{
			IOCFadeInterface::Execute_SetFadeOpacity(NewActor, 0.03f);
		}
	}
	OccludedActors = ActorsToHide;
}

