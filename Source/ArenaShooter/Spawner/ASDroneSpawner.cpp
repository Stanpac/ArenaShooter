// Fill out your copyright notice in the Description page of Project Settings.


#include "ASDroneSpawner.h"


AASDroneSpawner::AASDroneSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AASDroneSpawner::SpawnAI()
{
	if (m_currentSpawnedActors.Num() >= m_MaxSpawnCount) return; 

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* Actor = GetWorld()->SpawnActor<AActor>(m_AIPawnClassTOSpawn, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
	if (Actor != nullptr) {
		m_currentSpawnedActors.Add(Actor);
		Actor->OnDestroyed.AddDynamic(this, &AASDroneSpawner::OnCurrentSpawnedActorDestroyed);
		m_SpawnCount++;
		if (m_SpawnCount >= m_NbrOfDroneToSpawn) {
			SetCanSpawn(false);
			m_SpawnCount = 0;
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AASDroneSpawner::CanSpawnAgain, m_SpawnDelay, false);
		}
	}
}

void AASDroneSpawner::OnCurrentSpawnedActorDestroyed(AActor* DestroyedActor)
{
	m_currentSpawnedActors.Remove(DestroyedActor);
}

void AASDroneSpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsCanSpawn()) SpawnAI();
}

void AASDroneSpawner::BeginPlay()
{
	Super::BeginPlay();
	SetCanSpawn(true);
}

void AASDroneSpawner::CanSpawnAgain()
{
	SetCanSpawn(true);
}
