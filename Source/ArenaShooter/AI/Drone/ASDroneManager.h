// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ASDroneManager.generated.h"

UCLASS()
class ARENASHOOTER_API UASDroneManager : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<AActor*> m_Drones;

	
public:
	TArray<AActor*> GetDrones() const{ return m_Drones;}

	void AddDrone(AActor* drone) { m_Drones.Add(drone);};
};
