// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedProfilsData.h"

FSpeedProfile USpeedProfilsData::GetSpeedProfile(int SpeedProfile)
{
	if (m_SpeedProfiles.IsValidIndex(SpeedProfile)) {
		return m_SpeedProfiles[SpeedProfile];
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SpeedProfile is not in the Map!, returning default SpeedProfile!"));
	return FSpeedProfile();
}
