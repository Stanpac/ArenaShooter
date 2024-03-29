// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Character/ASCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ArenaShooter/Components/ASDashComponent.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "ArenaShooter/Components/GravitySwitchComponent.h"
#include "ArenaShooter/SubSystem/ASEventWorldSubSystem.h"
#include "ArenaShooter/Widget/ASGlobalWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


AASCharacter::AASCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	
	// Character Movement (Lyra Params)
	UCharacterMovementComponent* MoveComp = CastChecked<UCharacterMovementComponent>(GetCharacterMovement());
	MoveComp->GravityScale = 1.0f;
	MoveComp->MaxAcceleration = 2400.0f;
	MoveComp->BrakingFrictionFactor = 1.0f;
	MoveComp->BrakingFriction = 6.0f;
	MoveComp->GroundFriction = 8.0f;
	MoveComp->BrakingDecelerationWalking = 1400.0f;
	MoveComp->bUseControllerDesiredRotation = true;
	MoveComp->bOrientRotationToMovement = false;
	MoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	
	m_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	m_SpringArmComponent->SetupAttachment(CapsuleComp);
	m_SpringArmComponent->bUsePawnControlRotation = true;
	m_SpringArmComponent->TargetArmLength = 0.f;
	
	m_FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	m_FirstPersonCameraComponent->SetupAttachment(m_SpringArmComponent, USpringArmComponent::SocketName);
	
	m_Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	m_Mesh1P->SetOnlyOwnerSee(true);
	m_Mesh1P->SetupAttachment(m_FirstPersonCameraComponent);
	m_Mesh1P->bCastDynamicShadow = false;
	m_Mesh1P->CastShadow = false;
	
	m_Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Actor Components 
	m_HealthComponent = CreateDefaultSubobject<UASHealthComponent>(TEXT("HealthComponent"));
	m_WeaponComponent = CreateDefaultSubobject<UASWeaponComponent>(TEXT("WeaponComponent"));
	m_GravitySwitchComponent = CreateDefaultSubobject<UGravitySwitchComponent>(TEXT("GravitySwitchComponent"));
	m_DashComponent = CreateDefaultSubobject<UASDashComponent>(TEXT("DashComponent"));
}

void AASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &AASCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Triggered, this, &AASCharacter::Look);

		// Shooting
		EnhancedInputComponent->BindAction(m_ShootAction, ETriggerEvent::Triggered, this, &AASCharacter::Shoot);

		// Reload
		//EnhancedInputComponent->BindAction(m_ReloadAction, ETriggerEvent::Triggered, this, &AASCharacter::Reload);

		// CloseCombat
		//EnhancedInputComponent->BindAction(m_CloseCombatAction, ETriggerEvent::Triggered, this, &AASCharacter::CloseCombat);

		//GravitySwitch
		EnhancedInputComponent->BindAction(m_switchGravityAction, ETriggerEvent::Triggered, this, &AASCharacter::SwitchGravity);

		//Dash
		EnhancedInputComponent->BindAction(m_DashAction, ETriggerEvent::Triggered, this, &AASCharacter::Dash);
		
	} else {
		UE_LOG(LogTemp, Error, TEXT("'%s' Don't Find EnhancedInputComponent."), *GetNameSafe(this));
	}
}

void AASCharacter::DebugDamage(float amount)
{
	if (m_HealthComponent) {
		m_HealthComponent->Damage(amount, GetOwner());
	}
}

void AASCharacter::DebugHealing(float amount)
{
	if (m_HealthComponent) {
		m_HealthComponent->healing(amount);
	}
}

void AASCharacter::BeginPlay()
{
	Super::BeginPlay();
	AddDefaultMappingContext();
	GetAllSubsystem();
	
	M_PlayerWidget = CreateWidget<UASGlobalWidget>(GetWorld(), M_PlayerWidgetClass);
	if (M_PlayerWidget) {
		M_PlayerWidget->AddToViewport();
		GetPlayerWidget()->SetNbrOfChargeText(m_GravitySwitchComponent->GetNbrOfCharge());
	}

	m_HealthComponent->OnDeathStarted.AddDynamic(this, &AASCharacter::OnStartDeath);
	m_HealthComponent->OnHealthChanged.AddDynamic(this, &AASCharacter::OnHealthChanged);
	
	m_WeaponComponent->InitializeWeapon();
	m_WeaponComponent->OnFireEvent.AddDynamic(this, &AASCharacter::OnFire);

	m_GravitySwitchComponent->OnStartSwitchGravity.AddDynamic(this, &AASCharacter::OnChangeGravity);
	m_GravitySwitchComponent->OnSwitchGravityAbiltyCooldownEnd.AddDynamic(this, &AASCharacter::OnAbilityCooldownEnd);
	m_GravitySwitchComponent->OnGravityChargeRefill.AddDynamic(this, &AASCharacter::OnGravityChargeRefill);
}

void AASCharacter::GetAllSubsystem()
{
	m_EventWorldSubSystem = GetWorld()->GetSubsystem<UASEventWorldSubSystem>();
}

void AASCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr) {
		if (MovementVector.X != 0.0f) {
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}

		if (MovementVector.Y != 0.0f) {
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		}
	}
}

void AASCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (m_GravitySwitchComponent) {
		if (GetGravityDirection() != m_GravitySwitchComponent->GetBaseGravityDirection()) {
			LookAxisVector = FVector2D(-LookAxisVector.X, -LookAxisVector.Y);
		}
	}
	
	if (LookAxisVector.X != 0.0f) {
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.0f) {
		m_SpringArmComponent->AddRelativeRotation(FRotator(FMath::Clamp(-LookAxisVector.Y, -85, 85), 0.0f, 0.0f));
	}
}

void AASCharacter::Shoot(const FInputActionValue& Value)
{
	//TODO Take into account if the weapon allows maintaining the input or not
	m_WeaponComponent->Fire(m_FirstPersonCameraComponent->GetComponentLocation(), m_FirstPersonCameraComponent->GetForwardVector());
}

void AASCharacter::Reload(const FInputActionValue& Value)
{
	m_WeaponComponent->Reload();
}

void AASCharacter::SwitchGravity(const FInputActionValue& Value)
{
	if (m_GravitySwitchComponent) {
		m_GravitySwitchComponent->SwitchGravity();
	}
}

void AASCharacter::Dash(const FInputActionValue& Value)
{
	m_DashComponent->OnDash();
}

void AASCharacter::OnStartDeath(AActor* OwningActor)
{
	if (m_EventWorldSubSystem) {
		m_EventWorldSubSystem->BroadcastPlayerStartDeath();
	}
	
	if (Controller) {
		Controller->SetIgnoreMoveInput(true);
	}
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->StopMovementImmediately();
	MoveComp->DisableMovement();
	
	if(m_SoundDeath) {
		UGameplayStatics::PlaySoundAtLocation( GetWorld(), m_SoundDeath, GetOwner()->GetActorLocation());
	}
	
	// TODO : Add Death Animation and Call OnEndDeath at the end of the animation
	OnEndDeath();
}

void AASCharacter::OnEndDeath()
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	SetLifeSpan(0.1f);
	SetActorHiddenInGame(true);
	
	if (m_EventWorldSubSystem) {
		m_EventWorldSubSystem->BroadcastPlayerEndDeath();
	}
}

void AASCharacter::OnFire()
{
	UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;
	
	const float MontageLength = AnimInstance->Montage_Play(m_FireMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
}

void AASCharacter::OnChangeGravity()
{
	// Call when the Gravity Switch Start
	GetPlayerWidget()->SetGravityAbilityImageVisibility(false);
}

void AASCharacter::OnAbilityCooldownEnd()
{
	//GetPlayerWidget()->SetNbrOfChargeText(m_GravitySwitchComponent->GetNbrOfCharge());
	GetPlayerWidget()->SetGravityAbilityImageVisibility(true);
}

void AASCharacter::OnGravityChargeRefill()
{
	GetPlayerWidget()->SetGravityChargeBarPercent(m_GravitySwitchComponent->GetTimer() / m_GravitySwitchComponent->GetGravityChargeRefillTime());
	GetPlayerWidget()->SetNbrOfChargeText(m_GravitySwitchComponent->GetNbrOfCharge());
}

void AASCharacter::OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth,AActor* DamageDealer)
{
	GetPlayerWidget()->SethealthBarPercent(CurrentHealth / MaxHealth);
	if (CurrentHealth < PreviousHealth) {
		CheckPlayScreenShake();
		if(m_SoundHit) {
			UGameplayStatics::PlaySoundAtLocation( GetWorld(), m_SoundHit,GetOwner()->GetActorLocation());
		}
	}
}

void AASCharacter::CheckPlayScreenShake()
{
	if (!bPlayShakeOntakingDamage || m_ShakeClass == nullptr) {
		return;
	}
	
	APlayerCameraManager* lCamMgr = UGameplayStatics::GetPlayerCameraManager(this->GetWorld(), 0);
	if (lCamMgr != nullptr) {
		lCamMgr->StartCameraShake(m_ShakeClass);
	}
}

void AASCharacter::AddDefaultMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))  {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(m_DefaultMappingContext, 0);
		}
	}
}

void AASCharacter::RemoveDefaultMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))  {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(m_DefaultMappingContext);
		}
	}
}
	


