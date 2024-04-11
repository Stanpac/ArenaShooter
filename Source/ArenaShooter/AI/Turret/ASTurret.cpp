// Fill out your copyright notice in the Description page of Project Settings.


#include "ASTurret.h"

#include "NiagaraComponent.h"
#include "ArenaShooter/Character/ASCharacter.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/BillboardComponent.h"
#include "ArenaShooter/Widget/ASTurretWidget.h"
#include "GameFramework/PawnMovementComponent.h"


AASTurret::AASTurret(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	m_RotationRootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RotationRootPoint"));
	m_RotationRootPoint->SetupAttachment(m_RootComponent);
	
	m_MovingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MovingMeshComponent"));
	m_MovingMeshComponent->SetupAttachment(m_RotationRootPoint);
	
	m_ShootingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootingPoint"));
	m_ShootingPoint->SetupAttachment(m_MovingMeshComponent);

	m_TurretLaserParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TurretLaserParticleSystem"));
	m_TurretLaserParticleSystem->SetupAttachment(m_ShootingPoint);

#if WITH_EDITOR
	m_DebugShootPoint = CreateDefaultSubobject<UBillboardComponent>(TEXT("DebugShootPoint"));
	m_DebugShootPoint->SetupAttachment(m_ShootingPoint);
#endif
	
}

void AASTurret::BeginPlay()
{
	Super::BeginPlay();
	if (m_WidgetClass) {
		m_Widget = CreateWidget<UASTurretWidget>(GetWorld(), m_WidgetClass);
		m_Widget->AddToViewport();
		m_Widget->SetHealthBarPercent(m_HealthComponent->GetHealth(), m_HealthComponent->GetMaxHealth());
	}
	m_MovingMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AASTurret::OnOverlapBegin);
	m_CurrentPhase = 0;
	m_HealthComponent->SetIsInvincible(true);
}

void AASTurret::OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer)
{
	Super::OnHealthChanged(PreviousHealth, CurrentHealth, MaxHealth, DamageDealer);
	
	if (PreviousHealth > CurrentHealth) {
		if (GetWorldTimerManager().IsTimerActive(m_TakeDamageTimerHandle)) {
			GetWorldTimerManager().ClearTimer(m_TakeDamageTimerHandle);
		}
		GetWorldTimerManager().SetTimer(m_TakeDamageTimerHandle, this, &AASTurret::EndTakeDamage, m_TimeToResetTakeDamage, false);
		m_TurretState = ETurretState::ETS_Defence;
	}

	if (m_Widget) {
		m_Widget->SetHealthBarPercent(CurrentHealth, MaxHealth);
	}

	if(CurrentHealth == 1 && m_HealthComponent->GetIsInvincible()) {
		ChangePhase();
	}
}

void AASTurret::Shoot()
{
	m_WeaponComponent->Fire(m_ShootingPoint->GetComponentLocation(), m_ShootingPoint->GetForwardVector());
}

void AASTurret::EndTakeDamage()
{
	if (m_TurretState == ETurretState::ETS_Healing) {
		return;
	}
	m_TurretState = ETurretState::ETS_Attack;
}

void AASTurret::ChangePhase()
{
	m_CurrentPhase++;
	if (m_CurrentPhase > m_TurretPhases.Num()) {
		return;
	}
	m_TurretState = ETurretState::ETS_Healing;
	m_HealthComponent->SetIsDamageable(false);
	m_HealthComponent->SetMaxHealth(m_TurretPhases[m_CurrentPhase].m_TurrerHPMax);
	GetWorldTimerManager().SetTimer(m_HealingTimerHandle, this, &AASTurret::Healing, 0.1f, true);
}

void AASTurret::Healing()
{
	m_HealthComponent->healing(m_HealthComponent->GetMaxHealth() / 50 );
	if (m_HealthComponent->GetHealth() == m_HealthComponent->GetMaxHealth()) {
		GetWorldTimerManager().ClearTimer(m_HealingTimerHandle);
		m_HealthComponent->SetIsDamageable(true);
		if (m_CurrentPhase == m_TurretPhases.Num() - 1) {
			m_HealthComponent->SetIsInvincible(false);
		}
		m_TurretState = ETurretState::ETS_Attack;
	}
}

void AASTurret::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AASCharacter>()) {
		AASCharacter* lCharacter = Cast<AASCharacter>(OtherActor);
		lCharacter->LaunchCharacter(lCharacter->GetActorLocation()-GetActorLocation() * m_BumpForce, true, true);
	}
}
