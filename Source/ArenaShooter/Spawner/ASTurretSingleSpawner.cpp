// Fill out your copyright notice in the Description page of Project Settings.


#include "ASTurretSingleSpawner.h"

#include "ArenaShooter/Character/ASCharacter.h"
#include "Components/SphereComponent.h"


AASTurretSingleSpawner::AASTurretSingleSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	m_SpawnZone = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnZone"));
	m_SpawnZone->SetSphereRadius(1000.0f);
}

void AASTurretSingleSpawner::SpawnAI()
{
	if (m_CurrentActorSpawned != nullptr) return; // prevent from spawning multiple actors

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* Actor = GetWorld()->SpawnActor<AActor>(m_AIPawnClassTOSpawn.Get(), GetActorLocation() + m_SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	if (Actor != nullptr) {
		CanSpawn = false;
		m_CurrentActorSpawned = Actor;
		m_CurrentActorSpawned->OnDestroyed.AddDynamic(this, &AASTurretSingleSpawner::OnCurrentSpawnedActorDestroyed);
	}
}

void AASTurretSingleSpawner::OnCurrentSpawnedActorDestroyed(AActor* DestroyedActor)
{
	m_CurrentActorSpawned = nullptr;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AASTurretSingleSpawner::CanSpawnAgain, m_SpawnDelay, false);
}

void AASTurretSingleSpawner::BeginPlay()
{
	Super::BeginPlay();
	m_SpawnZone->OnComponentBeginOverlap.AddDynamic(this, &AASTurretSingleSpawner::OnComponentBeginOverlap);
}

void AASTurretSingleSpawner::CanSpawnAgain()
{
	CanSpawn = true;
}

void AASTurretSingleSpawner::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanSpawn) return;
	
	if (AASCharacter* Character =  Cast<AASCharacter>(OtherActor)) {
		SpawnAI();
	}
}





