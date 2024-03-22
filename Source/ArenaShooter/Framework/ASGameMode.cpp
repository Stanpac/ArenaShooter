// Fill out your copyright notice in the Description page of Project Settings.


#include "ASGameMode.h"

#include "ArenaShooter/ASAI_CharacterDetectionPathfinder.h"
#include "ArenaShooter/SubSystem/ASEventWorldSubSystem.h"
#include "Kismet/GameplayStatics.h"


void AASGameMode::BeginPlay()
{
	Super::BeginPlay();
	UASAI_CharacterDetectionPathfinder* pathfinder = GetWorld()->GetSubsystem<UASAI_CharacterDetectionPathfinder>();
	pathfinder->m_dataAsset = m_PathfindingDataAsset;
	pathfinder->GeneratePathfindingData();
	m_EventWorldSubSystem = GetWorld()->GetSubsystem<UASEventWorldSubSystem>();
	m_EventWorldSubSystem->OnPlayerEndDeath.AddDynamic(this, &AASGameMode::RestartLevel);
}

void AASGameMode::RestartLevel()
{
	UWorld* World = GetWorld();
	
	// Get the name of the current level.
	FString CurrentLevelName = World->GetMapName();
	CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix);

	// Convert the name into a format that can be used for loading.
	FName LevelName(*CurrentLevelName);

	// Use the GameplayStatics class to load the level.
	UGameplayStatics::OpenLevel(World, LevelName);
}
