// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ASPlayerCameraManager.generated.h"

/**
 *  Class that defines the Camera Manager for the Player
 */
UCLASS()
class ARENASHOOTER_API AASPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	/** 
	 * Called to give PlayerCameraManager a chance to adjust view rotation updates before they are applied. 
	 * e.g. The base implementation enforces view rotation limits using LimitViewPitch, et al. 
	 * @param DeltaTime - Frame time in seconds.
	 * @param OutViewRotation - In/out. The view rotation to modify.
	 * @param OutDeltaRot - In/out. How much the rotation changed this frame.
	 */
	virtual void ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;
};
