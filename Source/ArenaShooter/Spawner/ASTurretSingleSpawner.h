// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASTurretSingleSpawner.generated.h"

class APawn;
class USphereComponent;

UCLASS()
class ARENASHOOTER_API AASTurretSingleSpawner : public AActor
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS -------------------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "AI Pawn Class To Spawn"))
	TSubclassOf<APawn> m_AIPawnClassTOSpawn;

	UPROPERTY(EditAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Spawn Zone"))
	TObjectPtr<USphereComponent> m_SpawnZone;

	UPROPERTY(EditAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Spawn Location", MakeEditWidget = true))
	FVector m_SpawnLocation;
	
	UPROPERTY(VisibleAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Current Actor Spawned"))
	AActor* m_CurrentActorSpawned;

	UPROPERTY(EditAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Spawn Delay"))
	float m_SpawnDelay = 5.0f;

	UPROPERTY(VisibleAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Spawn Delay Timer"))
	bool CanSpawn = true;
	
	/* ---------------------------------- FUNCTION -------------------------------------- */
public:
	AASTurretSingleSpawner();

	void SpawnAI();

	UFUNCTION()
	void OnCurrentSpawnedActorDestroyed(AActor* DestroyedActor);

private:

	virtual void BeginPlay() override;
	
	void CanSpawnAgain();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
