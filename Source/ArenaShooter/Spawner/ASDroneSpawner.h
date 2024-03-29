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

	UPROPERTY(EditAnywhere, Category = "AASDroneSpawner", meta = (DisplayName = "Spawn Delay"))
	float m_SpawnDelay = 10.f;

	UPROPERTY(EditAnywhere, Category = "AASDroneSpawner", meta = (DisplayName = "Nbr Of Drone to Spawn at end of Spawn Delay"))
	int m_NbrOfDroneToSpawn = 3;
	
	UPROPERTY(EditAnywhere, Category = "AASDroneSpawner", meta = (DisplayName = "Max Drone Spawn by the Spawner"))
	int m_MaxSpawnCount = 15;

	UPROPERTY(VisibleAnywhere, Category = "AASDroneSpawner", meta = (DisplayName = "Current Actor Spawned"))
	TArray<AActor*> m_currentSpawnedActors;
	
	UPROPERTY(VisibleAnywhere, Category = "AASDroneSpawner", meta = (DisplayName = "Spawn Count"))
	int m_SpawnCount = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "AASDroneSpawner", meta = (DisplayName = "Spawn Delay Timer"))
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

	
	/* ---------------------------------- GETSET -------------------------------------- */
public :
	FORCEINLINE bool IsCanSpawn() const { return CanSpawn; }
	FORCEINLINE void SetCanSpawn(bool bCanSpawn) { CanSpawn = bCanSpawn; }
};
