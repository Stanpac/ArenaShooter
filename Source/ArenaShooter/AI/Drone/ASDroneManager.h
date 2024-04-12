// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ASDroneManager.generated.h"

class AASDronePawn;

UCLASS()
class ARENASHOOTER_API UASDroneManager : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<AActor*> m_Drones;

	
public:
	TArray<AActor*> GetDrones() const{ return m_Drones;}

	void AddDrone(AActor* drone) ;

	UFUNCTION()
	void RemoveDrones(AActor* destroyedDrone) ;
};
