// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Character/ASCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NinjaCharacterMovementComponent.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/SubSystem/ASEventWorldSubSystem.h"
#include "ArenaShooter/Widget/ASGlobalWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AASCharacter::AASCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	MeshComp->SetRelativeLocation(FVector(0, 0, -90));
	MeshComp->SetupAttachment(GetCapsuleComponent());

	// Character Movement (Lyra Params)
	UCharacterMovementComponent* MoveComp = CastChecked<UCharacterMovementComponent>(GetCharacterMovement());
	MoveComp->GravityScale = 1.0f;
	MoveComp->MaxAcceleration = 2400.0f;
	MoveComp->BrakingFrictionFactor = 1.0f;
	MoveComp->BrakingFriction = 6.0f;
	MoveComp->GroundFriction = 8.0f;
	MoveComp->BrakingDecelerationWalking = 1400.0f;
	MoveComp->bUseControllerDesiredRotation = false;
	MoveComp->bOrientRotationToMovement = false;
	MoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	MoveComp->SetCrouchedHalfHeight(65.0f);

	// Ninja Component (Temp)
	UNinjaCharacterMovementComponent* NinjaMoveComp = CastChecked<UNinjaCharacterMovementComponent>(GetCharacterMovement());
	NinjaMoveComp->SetAlignGravityToBase(true);
	NinjaMoveComp->SetAlignComponentToGravity(true);
	NinjaMoveComp->bAlwaysRotateAroundCenter = true;

	m_FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	m_FirstPersonCameraComponent->SetupAttachment(MeshComp, TEXT("head"));
	m_FirstPersonCameraComponent->SetRelativeLocation(FVector(0, 10.f, 0));
	m_FirstPersonCameraComponent->SetRelativeRotation(FRotator(-90, 0, 90));
	m_FirstPersonCameraComponent->bUsePawnControlRotation = true;

	m_HealthComponent = CreateDefaultSubobject<UASHealthComponent>(TEXT("HealthComponent"));
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
		
	} else {
		UE_LOG(LogTemp, Error, TEXT("'%s' Don't Find EnhancedInputComponent."), *GetNameSafe(this));
	}
}

void AASCharacter::DebugDamage(float amount)
{
	if (m_HealthComponent) {
		m_HealthComponent->Damage(amount);
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
	}
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
	//Shoot system
}

void AASCharacter::OnStartDeath()
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

	UCharacterMovementComponent* LyraMoveComp = GetCharacterMovement();
	LyraMoveComp->StopMovementImmediately();
	LyraMoveComp->DisableMovement();
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



