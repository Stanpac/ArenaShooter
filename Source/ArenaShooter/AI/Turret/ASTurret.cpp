// Fill out your copyright notice in the Description page of Project Settings.


#include "ASTurret.h"

#include "NiagaraComponent.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "Components/BillboardComponent.h"


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
}

void AASTurret::OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer)
{
	Super::OnHealthChanged(PreviousHealth, CurrentHealth, MaxHealth, DamageDealer);
	if (PreviousHealth > CurrentHealth) {
		m_HaveTakeDamageRecently = true;
		if (GetWorldTimerManager().IsTimerActive(m_TakeDamageTimerHandle)) {
			GetWorldTimerManager().ClearTimer(m_TakeDamageTimerHandle);
		}
		GetWorldTimerManager().SetTimer(m_TakeDamageTimerHandle, this, &AASTurret::ResetTakeDamage, m_TimeToResetTakeDamage, false);
	}
}

void AASTurret::Shoot()
{
	m_WeaponComponent->Fire(m_ShootingPoint->GetComponentLocation(), m_ShootingPoint->GetForwardVector());
}

void AASTurret::ResetTakeDamage()
{
	m_HaveTakeDamageRecently = false;
}
