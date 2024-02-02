// Fill out your copyright notice in the Description page of Project Settings.


#include "ASGameMode.h"

#include "ASAI_CharacterDetectionPathfinder.h"

void AASGameMode::BeginPlay()
{
	Super::BeginPlay();
	UASAI_CharacterDetectionPathfinder* pathfinder = GetWorld()->GetSubsystem<UASAI_CharacterDetectionPathfinder>();
	pathfinder->m_dataAsset = m_PathfindingDataAsset;
	pathfinder->GeneratePathfindingData();
	
}
