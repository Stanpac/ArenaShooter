// Fill out your copyright notice in the Description page of Project Settings.


#include "ASTurret.h"

#include "ArenaShooter/Components/ASHealthComponent.h"
#include "Components/BillboardComponent.h"


AASTurret::AASTurret(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	m_RotationRootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RotationRootPoint"));
	m_RotationRootPoint->SetupAttachment(RootComponent);
	
	m_MovingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MovingMeshComponent"));
	m_MovingMeshComponent->SetupAttachment(m_RotationRootPoint);
	
	m_ShootingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootingPoint"));
	m_ShootingPoint->SetupAttachment(m_MovingMeshComponent);

	// Put With editor Only
	m_DebugShootPoint = CreateDefaultSubobject<UBillboardComponent>(TEXT("DebugShootPoint"));
	m_DebugShootPoint->SetupAttachment(m_ShootingPoint);
	// End Put With editor Only
}

void AASTurret::BeginPlay()
{
	Super::BeginPlay();
}

void AASTurret::OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer)
{
	Super::OnHealthChanged(PreviousHealth, CurrentHealth, MaxHealth, DamageDealer);
	// When the Turret Take Damage We Temporary ++ the m_rotationSpeed
	
}
