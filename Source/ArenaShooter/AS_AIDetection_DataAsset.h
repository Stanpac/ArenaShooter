// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AS_AIDetection_DataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API UAS_AIDetection_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Pathfinding Data Creation", meta=(DisplayName= "Origin Offset"))
	FVector m_OriginOffset;
	
	UPROPERTY(EditAnywhere, Category = "Pathfinding Data Creation", meta=(DisplayName= "Rows"))
	int32 m_Rows; 
 
	UPROPERTY(EditAnywhere, Category = "Pathfinding Data Creation", meta=(DisplayName= "Columns"))
	int32 m_Columns; 
 
	UPROPERTY(EditAnywhere, Category = "Pathfinding Data Creation", meta=(DisplayName= "Steps (Z Axis)"))
	int32 m_Steps; 
	 
	UPROPERTY(EditAnywhere, Category = "Pathfinding Data Creation", meta=(DisplayName= "Rows and Columns Spacing"))
	float m_Spacing; 
 
	UPROPERTY(EditAnywhere, Category = "Pathfinding Data Creation", meta=(DisplayName= "Steps Spacing"))
	float m_StepSpacing;

	UPROPERTY(EditAnywhere, Category = "Collision Data", meta=(DisplayName= "Collision Detection Radius"))
	float m_CollisionDetectionRadius;

	UPROPERTY(EditAnywhere, Category = "Detection", meta=(DisplayName= "Distance To Validate DetectionPoint"))
	float m_DistanceToValidateDetectionPoint;
	
	UPROPERTY(EditAnywhere, Category = "Debug", meta=(DisplayName= "Debug Duration"))
	float m_DebugSphereDuration;
};
