// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpeedProfilsData.generated.h"

USTRUCT(BlueprintType)
struct FSpeedProfile
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,meta = (DisplayName = "Speed Profile", ClampMin = 0, ClampMax = 3));
	int m_SpeedProfile;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Acceleration"));
	float m_MaxAcceleration = 3000.f;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Walk Speed"));
	float m_MaxWalkSpeed = 1500.f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Mass"));
	float m_Mass = 100.f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Gravity"));
	float m_Gravity = 4.f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Jump Z"));
	float m_JumpZ = 1600.f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Air Control"));
	float m_AirControl = 0.82f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Breaking Deceleration Falling"));
	float m_BreakingDecelerationFalling = 4000.f;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "SpeedBarDecreasRated"));
	float m_SpeedBarDecreasRate = 10.f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "SpeedBar Value Max"));
	float m_SpeedBarValueMax = 1000.f;
};

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API USpeedProfilsData : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FSpeedProfile> m_SpeedProfiles;

public:
	FSpeedProfile GetSpeedProfile(int SpeedProfile);

	FORCEINLINE int GetSpeedProfileCount() const { return m_SpeedProfiles.Num(); }
};
