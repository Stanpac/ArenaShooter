// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Character/ASCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ArenaShooter/Components/ASCloseCombatComponent.h"
#include "ArenaShooter/Components/ASDashComponent.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASSpeedComponent.h"
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
	m_CloseCombatComponent = CreateDefaultSubobject<UASCloseCombatComponent>(TEXT("CloseCombatComponent"));
	m_SpeedComponent = CreateDefaultSubobject<UASSpeedComponent>(TEXT("SpeedComponent"));
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
		EnhancedInputComponent->BindAction(m_ReloadAction, ETriggerEvent::Triggered, this, &AASCharacter::Reload);

		// CloseCombat
		EnhancedInputComponent->BindAction(m_CloseCombatAction, ETriggerEvent::Triggered, this, &AASCharacter::CloseCombat);

		//GravitySwitch
		EnhancedInputComponent->BindAction(m_switchGravityAction, ETriggerEvent::Triggered, this, &AASCharacter::SwitchGravity);

		//Dash
		EnhancedInputComponent->BindAction(m_DashAction, ETriggerEvent::Triggered, this, &AASCharacter::Dash);
		
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
	m_WeaponComponent->OnFireEvent.AddDynamic(this, &AASCharacter::OnFire);

	m_CloseCombatComponent->OnStartCloseCombatAttack.AddDynamic(this, &AASCharacter::OnAttack);

	m_GravitySwitchComponent->OnStartSwitchGravity.AddDynamic(this, &AASCharacter::OnChangeGravity);
	m_GravitySwitchComponent->OnSwitchGravityAbiltyCooldownEnd.AddDynamic(this, &AASCharacter::OnAbilityCooldownEnd);
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

void AASCharacter::Switch(const FInputActionValue& Value) 
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
	UWorld* World = GetWorld();
	if (!World) return;

	// Get the name of the current level.
	FString CurrentLevelName = World->GetMapName();
	CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix);

	// Convert the name into a format that can be used for loading.
	FName LevelName(*CurrentLevelName);

	// Use the GameplayStatics class to load the level.
	UGameplayStatics::OpenLevel(World, LevelName);
	if (m_EventWorldSubSystem) {
		m_EventWorldSubSystem->BroadcastPlayerEndDeath();
	}
	
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

	GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, TEXT("DIE"));
	//end
	UWorld* World = GetWorld();
	if (!World) return;

	// Get the name of the current level.
	FString CurrentLevelName = World->GetMapName();
	CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix);

	// Convert the name into a format that can be used for loading.
	FName LevelName(*CurrentLevelName);

	// Use the GameplayStatics class to load the level.
	UGameplayStatics::OpenLevel(World, LevelName);
	if (m_EventWorldSubSystem) {
		m_EventWorldSubSystem->BroadcastPlayerEndDeath();
	}
	//SetLifeSpan(0.1f);
	//SetActorHiddenInGame(true);
}

void AASCharacter::OnFire()
{
	UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;
	
	const float MontageLength = AnimInstance->Montage_Play(m_FireMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
}

void AASCharacter::OnAttack()
{
	UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;
	
	const float MontageLength = AnimInstance->Montage_Play(m_CacAttackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
}

void AASCharacter::OnChangeGravity()
{
	// Call when the Gravity Switch Start
	GetPlayerWidget()->SetGravityAbilityImageVisibility(false);
}

void AASCharacter::OnAbilityCooldownEnd()
{
	// Call when the Gravity Switch Cooldown End
	GetPlayerWidget()->SetGravityAbilityImageVisibility(true);
}

void AASCharacter::OnGravityChargeRefill()
{
	// Call when the Gravity Charge Refill (each 0.1f)
	GetPlayerWidget()->SetGravityChargeBarPercent(m_GravitySwitchComponent->GetTimer() / m_GravitySwitchComponent->GetGravityChargeRefillTime());
}

void AASCharacter::OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth,AActor* DamageDealer)
{
	GetPlayerWidget()->SethealthBarPercent(CurrentHealth / MaxHealth);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnHealthChanged"));
	if (CurrentHealth < PreviousHealth) {
		CheckPlayScreenShake();
	}
}

void AASCharacter::OnSpeedProfileChanged(int SpeedProfile)
{
	GetPlayerWidget()->SetSpeedProfile(SpeedProfile);
}

void AASCharacter::OnSpeedChanged(float NewSpeed, float MaxSpeed)
{
	GetPlayerWidget()->SetSpeedBarPercent(NewSpeed / MaxSpeed);
}

void AASCharacter::CheckPlayScreenShake()
{
	if (!bPlayShakeOntakingDamage || m_ShakeClass == nullptr) {
		return;
	}
	
	APlayerCameraManager* lCamMgr = UGameplayStatics::GetPlayerCameraManager(this->GetWorld(), 0);
	if (lCamMgr != nullptr) {
		lCamMgr->StartCameraShake(m_ShakeClass);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Shake"));
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
	


