// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerData.h"

FSpawnerParam USpawnerData::GetSpawnerData(int SpawnerDataIndex)
{
	if (m_SpawnerData.IsValidIndex(SpawnerDataIndex)) {
		return m_SpawnerData[SpawnerDataIndex];
	}
	return FSpawnerParam();
}
