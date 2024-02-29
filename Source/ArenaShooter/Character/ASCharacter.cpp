// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Character/ASCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ArenaShooter/Components/ASCloseCombatComponent.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASSpeedComponent.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "ArenaShooter/SubSystem/ASEventWorldSubSystem.h"
#include "ArenaShooter/Widget/ASGlobalWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AASCharacter::AASCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	
	// Character Movement (Lyra Params)
	/*UCharacterMovementComponent* MoveComp = CastChecked<UCharacterMovementComponent>(GetCharacterMovement());
	MoveComp->GravityScale = 1.0f;
	MoveComp->MaxAcceleration = 2400.0f;
	MoveComp->BrakingFrictionFactor = 1.0f;
	MoveComp->BrakingFriction = 6.0f;
	MoveComp->GroundFriction = 8.0f;
	MoveComp->BrakingDecelerationWalking = 1400.0f;
	MoveComp->bUseControllerDesiredRotation = true;
	MoveComp->bOrientRotationToMovement = false;
	MoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;*/
	
	m_FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	m_FirstPersonCameraComponent->SetupAttachment(CapsuleComp);
	m_FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 40.f));
	m_FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	m_Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	m_Mesh1P->SetOnlyOwnerSee(true);
	m_Mesh1P->SetupAttachment(m_FirstPersonCameraComponent);
	m_Mesh1P->bCastDynamicShadow = false;
	m_Mesh1P->CastShadow = false;
	
	m_Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Actor Components 
	m_HealthComponent = CreateDefaultSubobject<UASHealthComponent>(TEXT("HealthComponent"));
	m_WeaponComponent = CreateDefaultSubobject<UASWeaponComponent>(TEXT("WeaponComponent"));
	m_CloseCombatComponent = CreateDefaultSubobject<UASCloseCombatComponent>(TEXT("CloseCombatComponent"));
	m_SpeedComponent = CreateDefaultSubobject<UASSpeedComponent>(TEXT("SpeedComponent"));

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
		EnhancedInputComponent->BindAction(m_ReloadAction, ETriggerEvent::Triggered, this, &AASCharacter::Reload);

		// CloseCombat
		EnhancedInputComponent->BindAction(m_CloseCombatAction, ETriggerEvent::Triggered, this, &AASCharacter::CloseCombat);
		
		EnhancedInputComponent->BindAction(m_switchGravityAction, ETriggerEvent::Triggered, this, &AASCharacter::SwitchGravity);
		
		// Switching Weapon
		//EnhancedInputComponent->BindAction(m_switchWeaponAction, ETriggerEvent::Triggered, this, &AASCharacter::SwitchWeapon);
		
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

void AASCharacter::CheatSpeed(bool Cheat)
{
	SpeedCheatAllowed = Cheat;
}

void AASCharacter::BeginPlay()
{
	Super::BeginPlay();
	AddDefaultMappingContext();
	GetAllSubsystem();
	
	M_PlayerWidget = CreateWidget<UASGlobalWidget>(GetWorld(), M_PlayerWidgetClass);
	if (M_PlayerWidget) {
		M_PlayerWidget->AddToViewport();
	}

	m_SpeedComponent->OnUpdateSpeedProfile.AddDynamic(this, &AASCharacter::OnSpeedProfileChanged);
	m_SpeedComponent->OnUpdateSpeed.AddDynamic(this, &AASCharacter::OnSpeedChanged);

	m_HealthComponent->OnDeathStarted.AddDynamic(this, &AASCharacter::OnStartDeath);
	m_HealthComponent->OnHealthChanged.AddDynamic(this, &AASCharacter::OnHealthChanged);
	
	m_WeaponComponent->InitializeWeapon();
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

	if (Controller != nullptr) {
		if (LookAxisVector.X != 0.0f) {
			AddControllerYawInput(LookAxisVector.X);
		}

		if (LookAxisVector.Y != 0.0f) {
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void AASCharacter::Shoot(const FInputActionValue& Value)
{
	// Temp : Should not be here, just For testing
	if (SpeedCheatAllowed) {
		m_EventWorldSubSystem->BroadcastEnemyDeath();
	}
	//end
	
	//TODO Take into account if the weapon allows maintaining the input or not
	m_WeaponComponent->Fire(m_FirstPersonCameraComponent->GetComponentLocation(), m_FirstPersonCameraComponent->GetForwardVector());
}

void AASCharacter::Reload(const FInputActionValue& Value)
{
	m_WeaponComponent->Reload();
}

void AASCharacter::Switch(const FInputActionValue& Value) const
{
	m_WeaponComponent->SwitchWeapon();		
}

void AASCharacter::CloseCombat(const FInputActionValue& Value)
{
	if(m_CloseCombatComponent->m_CanAttack)
		m_CloseCombatComponent->StartCloseCombatAttack();
}

void AASCharacter::SwitchGravity(const FInputActionValue& Value)
{
	if (bIsSwitchingGravity) return;
	bIsSwitchingGravity = true;
	GetCharacterMovement()->SetGravityDirection(-GetGravityDirection());
}

void AASCharacter::OnStartDeath(AActor* OwningActor)
{
	//TODO : Should be moved In a Player Class if there is
	if (m_EventWorldSubSystem) {
		m_EventWorldSubSystem->BroadcastPlayerStartDeath();
	}
	//end
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
}

void AASCharacter::OnEndDeath()
{
	//TODO : Should be moved In a Player Class if there is
	if (m_EventWorldSubSystem) {
		m_EventWorldSubSystem->BroadcastPlayerEndDeath();
	}
	//end
	SetLifeSpan(0.1f);
	SetActorHiddenInGame(true);
}

void AASCharacter::OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth,AActor* DamageDealer)
{
	GetPlayerWidget()->SethealthBarPercent(CurrentHealth / MaxHealth);
}

void AASCharacter::OnSpeedProfileChanged(int SpeedProfile)
{
	GetPlayerWidget()->SetSpeedProfile(SpeedProfile);
}

void AASCharacter::OnSpeedChanged(float NewSpeed, float MaxSpeed)
{
	GetPlayerWidget()->SetSpeedBarPercent(NewSpeed / MaxSpeed);
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



