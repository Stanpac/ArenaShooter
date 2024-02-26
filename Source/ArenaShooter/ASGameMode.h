// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ASGameMode.generated.h"

class UAS_AIDetection_DataAsset;
/**
 * 
 */
UCLASS()
class ARENASHOOTER_API AASGameMode : public AGameMode
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere)
	UAS_AIDetection_DataAsset* m_PathfindingDataAsset;

	virtual void BeginPlay();
};
