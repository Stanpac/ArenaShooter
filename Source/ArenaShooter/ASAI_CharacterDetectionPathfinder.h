// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ASAI_CharacterDetectionPathfinder.generated.h"


class UAS_AIDetection_DataAsset;

USTRUCT()
struct FPathfindingPoint
{
	GENERATED_BODY()

	FPathfindingPoint()
	{
		m_IsInWall = true;
		m_DistanceToOrigin = 0;
		m_IsChecked = false;
		m_previousPoint = FInt32Vector(-1, 0, 0);
		m_Coordinates = FInt32Vector(-1, 0, 0);
	}
	
	FPathfindingPoint(FInt32Vector Coordinates)
	{
		m_IsInWall = true;
		m_DistanceToOrigin = 0;
		m_IsChecked = false;
		m_previousPoint = FInt32Vector(-1, 0, 0);
		m_Coordinates = Coordinates;
	}
	
	FPathfindingPoint(bool IsInWall, FInt32Vector Coordinates)
	{
		m_IsInWall = IsInWall;
		m_DistanceToOrigin = 0;
		m_IsChecked = false;
		m_previousPoint = FInt32Vector(-1, 0, 0);
		m_Coordinates = Coordinates;
	}

	UPROPERTY()
	bool m_IsInWall;

	UPROPERTY()
	float m_DistanceToOrigin;

	UPROPERTY()
	bool m_IsChecked;

	UPROPERTY()
	FInt32Vector m_previousPoint;

	UPROPERTY()
	FInt32Vector m_Coordinates;

};

UCLASS()
class ARENASHOOTER_API UASAI_CharacterDetectionPathfinder : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void GeneratePathfindingData();

	UFUNCTION()
	FInt32Vector GetCharacterDetectionPoint(const FVector& SourcePosition);

	UFUNCTION()
	FInt32Vector GetClosestPathfindingPoint(const FVector& SourcePosition) const;

	UFUNCTION()
	FVector GetWorldCoordinatesFromPathfindingPoint(FInt32Vector PathfindingPoint) const;
	
	UFUNCTION()
	TArray<FVector> GetPathToCharacterDetectionPoint(const FVector SourcePosition);

	UFUNCTION()
	TArray<FInt32Vector> CheckAllAdjacentPointsDetection(FInt32Vector previousPoint);
	
	UFUNCTION()
	void CheckAdjacentPointDetection(int32 x, int32 y, int32 z, FInt32Vector previousPoint, TArray<FInt32Vector>& currentPoints);

	UFUNCTION()
	bool IsNextPointValid(int32 x, int32 y, int32 z, FInt32Vector previousPoint);
	
public:
	UPROPERTY()
	UAS_AIDetection_DataAsset* m_dataAsset;

	UFUNCTION()
	FInt32Vector IndexToCoordinates(int32 index) const;

	UFUNCTION()
	int32 CoordinatesToIndex(FInt32Vector point) const;

	UFUNCTION()
	FVector PathfindingCoordinatesToWorldCoordinates(FInt32Vector point) const;
	
private:
	UPROPERTY()
	TArray<FPathfindingPoint> m_PathfindingData;

	UPROPERTY()
	bool m_IsPointFound;

	UPROPERTY()
	ACharacter* m_character;
	
public:
	TArray<FPathfindingPoint> GetPathfindingData()
	{
		return m_PathfindingData;
	}
};
