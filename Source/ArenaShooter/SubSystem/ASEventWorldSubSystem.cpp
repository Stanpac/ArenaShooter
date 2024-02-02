// Fill out your copyright notice in the Description page of Project Settings.


#include "ASEventWorldSubSystem.h"

void UASEventWorldSubSystem::BroadcastPlayerStartDeath()
{
	OnPlayerStartDeath.Broadcast();
}

void UASEventWorldSubSystem::BroadcastPlayerEndDeath()
{
	OnPlayerEndDeath.Broadcast();
}

void UASEventWorldSubSystem::BroadcastEnemyDeath()
{
	OnEnemyDeath.Broadcast();
}

void UASEventWorldSubSystem::BroadcastSpeedChange(int SpeedProfile)
{
	OnSpeedChange.Broadcast(SpeedProfile);
}
