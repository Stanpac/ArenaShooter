// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaShooter/SubSystem/ASEventWorldSubSystem.h"
#include "Components/ActorComponent.h"
#include "ASSpeedComponent.generated.h"


USTRUCT()
struct FSpeedProfile
{
	GENERATED_BODY()

	UPROPERTY(meta = (DisplayName = "Speed Profile", ClampMin = 0, ClampMax = 3));
	int m_SpeedProfile;

	float m_MaxAcceleration = 0.0f;
	float m_MaxWalkSpeed = 0.0f;
	float m_Mass = 0.0f;
	float m_Gravity = 0.0f;
    float m_JumpZ = 0.0f;
	float m_AirControl = 0.0f;
	float m_BreakingDecelerationFalling = 0.0f;
	float m_SpeedBarValueMax = 0.0f;
	float m_SpeedBarDecreasRate = 0.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENASHOOTER_API UASSpeedComponent : public UActorComponent
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	UPROPERTY(EditAnywhere, Category = "ASSpeedComponent|Profile", meta = (DisplayName = "Speed Profile"))
	TArray<FSpeedProfile> m_SpeedProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASSpeedComponent|Profile", meta = (DisplayName = "Current Speed Profile", ClampMin = 0, ClampMax = 3))
	int m_CurrentSpeedProfile;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ASSpeedComponent|Speed", meta = (DisplayName = "SpeedBarValue"))
	float m_SpeedBarValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ASSpeedComponent|Speed", meta = (DisplayName = "SpeedBarMaxValue"))
	float m_SpeedBarMaxValue;

	/** Speed Bar Amount of loss per Second */
	UPROPERTY(EditAnywhere)
	float m_SpeedBarDecreasRate = 10.0f;
	
	/** Event World SubSystem */
	UPROPERTY()
	UASEventWorldSubSystem* m_EventWorldSubSystem;
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:	
	UASSpeedComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	void UpdateSpeedProfile(int SpeedProfile);

	void UpdateSpeedBarValue(float Value);

	void UpdateSpeedBarMaxValue(float Value);
};
