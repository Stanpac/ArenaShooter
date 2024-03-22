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
		CanSpawn = false;
		m_currentSpawnedActors.Add(Actor);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AASDroneSpawner::CanSpawnAgain, m_SpawnDelay, false);
		Actor->OnDestroyed.AddDynamic(this, &AASDroneSpawner::OnCurrentSpawnedActorDestroyed);
	}
}

void AASDroneSpawner::OnCurrentSpawnedActorDestroyed(AActor* DestroyedActor)
{
	m_currentSpawnedActors.Remove(DestroyedActor);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AASDroneSpawner::CanSpawnAgain, m_SpawnDelay, false);
}

void AASDroneSpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CanSpawn) SpawnAI();
}

void AASDroneSpawner::BeginPlay()
{
	Super::BeginPlay();
	CanSpawn = true;
}

void AASDroneSpawner::CanSpawnAgain()
{
	CanSpawn = true;
}
