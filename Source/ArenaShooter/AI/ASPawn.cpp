// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/AI/ASPawn.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "ArenaShooter/SubSystem/ASEventWorldSubSystem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Blueprint/UserWidget.h"


AASPawn::AASPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	m_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	m_CapsuleComponent->SetCollisionProfileName("Pawn");

	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	m_MeshComponent->SetupAttachment(m_CapsuleComponent);

	m_HealthComponent = CreateDefaultSubobject<UASHealthComponent>(TEXT("HealthComponent"));
	m_WeaponComponent = CreateDefaultSubobject<UASWeaponComponent>(TEXT("WeaponComponent"));
}

void AASPawn::Stun(float stunDuration)
{
	m_IsStunned = true;
	m_StunTimer = stunDuration;
}

void AASPawn::BeginPlay()
{
	Super::BeginPlay();
	
	m_EventWorldSubSystem = GetWorld()->GetSubsystem<UASEventWorldSubSystem>();

	m_HealthComponent->OnHealthChanged.AddDynamic(this, &AASPawn::OnHealthChanged);
	m_HealthComponent->OnDeathStarted.AddDynamic(this, &AASPawn::OnDeath);

	m_WeaponComponent->InitializeWeapon();
	m_WeaponComponent->m_ASPawnOwner = this;
	
	if (m_IndicatorWidgetClass) {
		m_IndicatorWidget = CreateWidget<UUserWidget>(GetWorld(), m_IndicatorWidgetClass);
	}
	
	if(m_IndicatorWidget){
		m_IndicatorWidget->AddToViewport(-1);
		m_IndicatorWidget->SetColorAndOpacity(m_IndicatorBaseColor);
	}
}

void AASPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(m_IsStunned) StunTick(DeltaSeconds);
	ManageIndicatorWidget();
	
	
}



void AASPawn::OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer)
{
	if (PreviousHealth == CurrentHealth) return;
	
	// Find a Way to get the Indicate that you can one shoot the enemy
	// Oeverlay Mat ?
	
	SpawnFloatingDamage(GetActorLocation(), GetActorRotation(), PreviousHealth - CurrentHealth);
}

void AASPawn::OnDeath(AActor* DeathDealer)
{
	if (m_EventWorldSubSystem) {
		m_EventWorldSubSystem->BroadcastEnemyDeath();
	}
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_DeathParticle, GetActorLocation());
	
	SetLifeSpan(0.1f);
	SetActorHiddenInGame(true);
}

void AASPawn::StunTick(float DeltaTime)
{
	m_StunTimer -= DeltaTime;
	if(m_StunTimer <= 0)
	{
		m_IsStunned = false;
	}
}

void AASPawn::ManageIndicatorWidget()
{
	if(m_IndicatorWidget == nullptr) return;
	
	FVector2D lScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), GetActorLocation(), lScreenPosition);
	if (!WasRecentlyRendered(0.1f)) {
		m_IndicatorWidget->SetVisibility(ESlateVisibility::Visible);
	} else {
		m_IndicatorWidget->SetVisibility(ESlateVisibility::Hidden);
		GEngine->AddOnScreenDebugMessage(7, 0.1f, FColor::Red, "hidden");
	}
	
	FVector2D lDesiredSize =FVector2D(GEngine->GameViewport->Viewport->GetSizeXY()) - m_IndicatorWidget->GetDesiredSize();
	lScreenPosition.X = FMath::Clamp(lScreenPosition.X, 0, lDesiredSize.X);
	lScreenPosition.Y = FMath::Clamp(lScreenPosition.Y, 0, lDesiredSize.Y);
	
	m_IndicatorWidget->SetPositionInViewport(lScreenPosition);
}

void AASPawn::SpawnFloatingDamage(const FVector& SpawnLocation, const FRotator& SpawnRotation, const float Damage)

{
	if (GetWorld() == nullptr) {
		UE_LOG(LogTemp, Error,TEXT("%s, can't spawn floating damage. No context"), *GetNameSafe(this));
		return;
	}

	if (m_floatingDamageSystem == nullptr) {
		UE_LOG(LogTemp, Error,TEXT("%s, can't spawn floating damage. The system is null"), *GetNameSafe(this));
		return;
	}

	UNiagaraComponent* lNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_floatingDamageSystem, SpawnLocation, SpawnRotation, FVector::One(), true, true);
	
	FVector4 lDamageData = FVector4();

	lDamageData.X = SpawnLocation.X;
	lDamageData.Y = SpawnLocation.Y;
	lDamageData.Z = SpawnLocation.Z;
	lDamageData.W = Damage;
	
	/*Set the damage value into the system, it will use to compute where the texture number should take float*/
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4Value(lNiagaraComp, FName("DamageInfo"), 0, lDamageData, false);
}


