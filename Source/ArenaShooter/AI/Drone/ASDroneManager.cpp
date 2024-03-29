// Fill out your copyright notice in the Description page of Project Settings.


#include "ASDroneManager.h"

void UASDroneManager::AddDrone(AActor* drone)
{
	m_Drones.Add(drone);
	drone->OnDestroyed.AddDynamic(this, &UASDroneManager::RemoveDrones);
}

void UASDroneManager::RemoveDrones(AActor* destroyedDrone)
{
	if(m_Drones.Contains(destroyedDrone))
	{
		m_Drones.Remove(destroyedDrone);
	}
}
