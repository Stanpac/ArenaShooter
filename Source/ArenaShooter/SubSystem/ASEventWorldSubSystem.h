// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ASEventWorldSubSystem.generated.h"

/* Exemple of event to broadcast
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExempleEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExempleEventOneParam, int, ExempleParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FExempleEventTwoParams, int, ExempleParam1, int, ExempleParam2);
Etc...
 */

// Player
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSPeedChangeEvent, int, SpeedProfile);

/**
 *  CLass that will be used to broadcast events to the world 
 */
UCLASS()
class ARENASHOOTER_API UASEventWorldSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
public :

	UPROPERTY(BlueprintAssignable)
	FPlayerDeathEvent OnPlayerStartDeath;

	UPROPERTY(BlueprintAssignable)
	FPlayerDeathEvent OnPlayerEndDeath;

	UPROPERTY(BlueprintAssignable)
	FEnemyDeathEvent OnEnemyDeath;

	UPROPERTY(BlueprintAssignable)
	FSPeedChangeEvent OnSpeedChange;
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public :
	
	UFUNCTION(BlueprintCallable)
	void BroadcastPlayerStartDeath();

	UFUNCTION(BlueprintCallable)
	void BroadcastPlayerEndDeath();

	UFUNCTION(BlueprintCallable)
	void BroadcastEnemyDeath();

	UFUNCTION(BlueprintCallable)
	void BroadcastSpeedProfileChange(int SpeedProfile);
	
};
