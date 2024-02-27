// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASTurretSingleSpawner.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARENASHOOTER_API UASTurretSingleSpawner : public UActorComponent
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS -------------------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "AI Pawn Class To Spawn"))
	TSoftClassPtr<APawn> m_AIPawnClassTOSpawn;
	
	UPROPERTY(VisibleAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "AI Spawn"))
	APawn* m_AISpawn;

	UPROPERTY(EditAnywhere, Category = "UASTurretSingleSpawner", meta = (DisplayName = "Spawn Location"))
	FVector m_SpawnLocation;
	/* ---------------------------------- FUNCTION -------------------------------------- */
public:
	UASTurretSingleSpawner();


};
