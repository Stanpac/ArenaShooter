// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASDroneSpawner.generated.h"

UCLASS()
class ARENASHOOTER_API AASDroneSpawner : public AActor
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS -------------------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "AASDroneSpawner", meta = (DisplayName = "AI Pawn Class To Spawn"))
	TSubclassOf<APawn> m_AIPawnClassTOSpawn;

	UPROPERTY(EditAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Spawn Delay"))
	float m_SpawnDelay = 5.0f;

	UPROPERTY(EditAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Max Spawn Count"))
	int m_MaxSpawnCount = 5;

	UPROPERTY(VisibleAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Current Actor Spawned"))
	AActor* m_currentSpawnedActor;
	
	UPROPERTY(VisibleAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Spawn Count"))
	int m_SpawnCount = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Spawn Delay Timer"))
	bool CanSpawn = true;

	/* ---------------------------------- FUNCTION -------------------------------------- */
public:
	AASDroneSpawner();

	void SpawnAI();

	UFUNCTION()
	void OnCurrentSpawnedActorDestroyed(AActor* DestroyedActor);

	virtual void Tick(float DeltaSeconds) override;

private:
	void BeginPlay() override;

	void CanSpawnAgain();
	
	
};
