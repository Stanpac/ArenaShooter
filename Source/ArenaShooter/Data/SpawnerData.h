// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpawnerData.generated.h"

USTRUCT()
struct FSpawnerParam
{
	GENERATED_BODY()
	// TODO : Add the SpawnerData Members (Wait for the GD to provide the data)
};
/**
 *  holds the data for the Spawners
 */
UCLASS()
class ARENASHOOTER_API USpawnerData : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "SpawnerData")
	TArray<FSpawnerParam> m_SpawnerData;

	// Use TMap instead of TArray if Needed
	/*UPROPERTY(EditDefaultsOnly, Category = "SpawnerData")
	TMap<FSpawnerData> m_SpawnerData;*/

public:
	FORCEINLINE FSpawnerParam GetSpawnerData(int SpawnerDataIndex);

	FORCEINLINE int GetSpawnerDataCount() const { return m_SpawnerData.Num(); }
};
