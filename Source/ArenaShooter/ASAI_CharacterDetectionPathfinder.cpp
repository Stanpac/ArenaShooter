#include "ASAI_CharacterDetectionPathfinder.h"
#include "AS_AIDetection_DataAsset.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UASAI_CharacterDetectionPathfinder::GeneratePathfindingData()
{
	//FHitResult HitResult;
	//FCollisionQueryParams QueryParams;
	m_character = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));



	const int32 TotalPoints = m_dataAsset->m_Rows * m_dataAsset->m_Columns * m_dataAsset->m_Steps;
	m_PathfindingData.Reserve(TotalPoints);

	ParallelFor(TotalPoints, [&](int32 Index) {
		const int32 x = Index / (m_dataAsset->m_Columns *  m_dataAsset->m_Steps);
		const int32 y = (Index % (m_dataAsset->m_Columns *  m_dataAsset->m_Steps)) /  m_dataAsset->m_Steps;
		const int32 z = Index %  m_dataAsset->m_Steps;

		// Calculate world position based on grid position and interval
		const FVector PointLocation(x * m_dataAsset->m_Spacing, y * m_dataAsset->m_Spacing, z * m_dataAsset->m_StepSpacing);
		const FPathfindingPoint NewPathfindingPoint(false, FInt32Vector(x, y, z));
		DrawDebugSphere(GetWorld(),FVector(x,y,z),m_dataAsset->m_CollisionDetectionRadius,6, FColor::Green,true, m_dataAsset->m_DebugSphereDuration, 0, 1.0f);

		m_PathfindingData[Index] = NewPathfindingPoint;
	}, EParallelForFlags::Unbalanced);
	/*for (int i = 0; i < m_dataAsset->m_Rows; i++)
	{
		for(int j = 0; j < m_dataAsset->m_Columns; j++)
		{
			for(int k = 0; k < m_dataAsset->m_Steps; k++)
			{
				FVector Point = FVector(i * m_dataAsset->m_Spacing, j * m_dataAsset->m_StepSpacing, k * m_dataAsset->m_Spacing) + m_dataAsset->m_OriginOffset;

				//bool IsInWall = GetWorld()->SweepSingleByChannel(HitResult,Point,Point,FQuat::Identity,ECC_Visibility, FCollisionShape::MakeSphere(m_dataAsset->m_CollisionDetectionRadius), QueryParams);
				FPathfindingPoint NewPathfindingPoint;
				NewPathfindingPoint.m_DistanceToOrigin = 0;
				NewPathfindingPoint.m_IsInWall = true;
				m_PathfindingData.Add(FInt32Vector(i,j,k), NewPathfindingPoint);
				/*if(IsInWall)
				{#1#
				/*}
				else if(!IsInWall)
				{
					DrawDebugSphere(GetWorld(),Point,m_dataAsset->m_CollisionDetectionRadius,24, FColor::Red,false, m_dataAsset->m_DebugSphereDuration, 0, 1.0f);
				}#1#
			}
			
		}
	}*/
	
}

TArray<FVector> UASAI_CharacterDetectionPathfinder::GetPathToCharacterDetectionPoint(const FVector SourcePosition)
{
	FInt32Vector targetLocation = GetCharacterDetectionPoint(SourcePosition);
	const int32 targetLocationIndex = CoordinatesToIndex(targetLocation);
	TArray<FVector> array;
	while(m_PathfindingData[targetLocationIndex].m_previousPoint != FInt32Vector(-1, 0, 0))
	{
		array.Add(GetWorldCoordinatesFromPathfindingPoint(m_PathfindingData[targetLocationIndex].m_previousPoint));
		targetLocation = m_PathfindingData[targetLocationIndex].m_previousPoint;
	}
	return array;
}

FInt32Vector UASAI_CharacterDetectionPathfinder::GetCharacterDetectionPoint(const FVector& SourcePosition)
{
	const FInt32Vector sourcePoint = GetClosestPathfindingPoint(SourcePosition);
	const int32 sourcePointIndex = CoordinatesToIndex(sourcePoint);
	for(auto& point : m_PathfindingData)
	{
		point.m_IsChecked = false;
		point.m_previousPoint = FInt32Vector(-1, 0, 0);
	}
	m_PathfindingData[sourcePointIndex].m_IsChecked= true;
	m_IsPointFound = false;
	TArray<FInt32Vector> lastCheckedPoints ;
	do
	{
		lastCheckedPoints = CheckAllAdjacentPointsDetection(sourcePoint);
	}
	while(lastCheckedPoints.Num() > 0 && !m_IsPointFound);

	if(m_IsPointFound)
	{
		return lastCheckedPoints.Last();
	}
	else
	{
		return FInt32Vector(-1, 0, 0);
	}
}

FInt32Vector UASAI_CharacterDetectionPathfinder::GetClosestPathfindingPoint(const FVector& SourcePosition) const
{
	const int32 xPosition = FMath::Clamp(FMath::RoundToInt32((SourcePosition.X - m_dataAsset->m_OriginOffset.X) / m_dataAsset->m_Spacing), 0, m_dataAsset->m_Rows);
	const int32 yPosition = FMath::Clamp(FMath::RoundToInt32((SourcePosition.Y - m_dataAsset->m_OriginOffset.Y) / m_dataAsset->m_Spacing), 0, m_dataAsset->m_Columns);
	const int32 zPosition = FMath::Clamp(FMath::RoundToInt32((SourcePosition.Z - m_dataAsset->m_OriginOffset.Z) / m_dataAsset->m_StepSpacing), 0, m_dataAsset->m_Steps);
	return FInt32Vector(xPosition, yPosition, zPosition);
}

FVector UASAI_CharacterDetectionPathfinder::GetWorldCoordinatesFromPathfindingPoint(FInt32Vector PathfindingPoint) const
{
	const float xPosition = ( PathfindingPoint.X + m_dataAsset->m_OriginOffset.X) * m_dataAsset->m_Spacing;
	const float yPosition = ( PathfindingPoint.Y + m_dataAsset->m_OriginOffset.Y) * m_dataAsset->m_Spacing;
	const float zPosition = ( PathfindingPoint.Z + m_dataAsset->m_OriginOffset.Z) * m_dataAsset->m_StepSpacing;
	return FVector(xPosition, yPosition, zPosition);
}

TArray<FInt32Vector> UASAI_CharacterDetectionPathfinder::CheckAllAdjacentPointsDetection(FInt32Vector previousPoint)
{
	TArray<FInt32Vector> points;
	FInt32Vector direction = FInt32Vector(1, 0 , 0);
	if(IsNextPointValid(direction.X, direction.Y, direction.Z, previousPoint))
		CheckAdjacentPointDetection(direction.X, direction.Y, direction.Z, previousPoint, points);
	
	direction = FInt32Vector(-1, 0 , 0);
	if(IsNextPointValid(direction.X, direction.Y, direction.Z, previousPoint))
		CheckAdjacentPointDetection(direction.X, direction.Y, direction.Z, previousPoint, points);

	direction = FInt32Vector(0, 1 , 0);
	if(IsNextPointValid(direction.X, direction.Y, direction.Z, previousPoint))
		CheckAdjacentPointDetection(direction.X, direction.Y, direction.Z, previousPoint, points);

	direction = FInt32Vector(0, -1 , 0);
	if(IsNextPointValid(direction.X, direction.Y, direction.Z, previousPoint))
		CheckAdjacentPointDetection(direction.X, direction.Y, direction.Z, previousPoint, points);

	direction = FInt32Vector(0, 0 , 1);
	if(IsNextPointValid(direction.X, direction.Y, direction.Z, previousPoint))
		CheckAdjacentPointDetection(direction.X, direction.Y, direction.Z, previousPoint, points);

	direction = FInt32Vector(0, 0 , -1);
	if(IsNextPointValid(direction.X, direction.Y, direction.Z, previousPoint))
		CheckAdjacentPointDetection(direction.X, direction.Y, direction.Z, previousPoint, points);

	return points;
}

void UASAI_CharacterDetectionPathfinder::CheckAdjacentPointDetection(int32 x, int32 y, int32 z, FInt32Vector previousPoint, TArray<FInt32Vector>& currentPoints)
{
	FHitResult OutHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(m_character);
	
	const FInt32Vector NextPoint = FInt32Vector(x + previousPoint.X, y + previousPoint.Y, z + previousPoint.Z);
	FVector nextPoint = GetWorldCoordinatesFromPathfindingPoint(NextPoint);
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, nextPoint, m_character->GetActorLocation(), ECC_Visibility, QueryParams);
	currentPoints.Add(NextPoint);
	int32 NextPointIndex = CoordinatesToIndex(NextPoint);
	m_PathfindingData[NextPointIndex].m_previousPoint = previousPoint;
	if(bHit || FVector::Distance(nextPoint, m_character->GetActorLocation()) > m_dataAsset->m_DistanceToValidateDetectionPoint)
	{
		m_PathfindingData[NextPointIndex].m_IsChecked = true;
	}
	else
	{
		m_IsPointFound = true;
	}
}

bool UASAI_CharacterDetectionPathfinder::IsNextPointValid(int32 x, int32 y, int32 z, FInt32Vector previousPoint)
{
	if(m_IsPointFound) return false;
	const bool condition1 = x + previousPoint.X < 0 || x + previousPoint.X >= m_dataAsset->m_Rows;
	const bool condition2= y + previousPoint.Y < 0 || y + previousPoint.Y >= m_dataAsset->m_Columns;
	const bool condition3 = z + previousPoint.Z < 0 || z + previousPoint.Z >= m_dataAsset->m_Steps;
	if(condition1 || condition2 || condition3 ) return false;
	else
	{
		FHitResult OutHit;
		FCollisionQueryParams QueryParams;
		const FInt32Vector NextPoint = FInt32Vector(x + previousPoint.X, y + previousPoint.Y, z + previousPoint.Z);
		const int32 NextPointIndex = CoordinatesToIndex(NextPoint);
		
		const bool condition4 = !m_PathfindingData[NextPointIndex].m_IsChecked;
		const bool condition5 = !m_PathfindingData[NextPointIndex].m_IsInWall;
		//TODO make the raycast be a sphere cast (the drone needs to be able to go through not to have a sightline on the next point)
		const bool condition6 =  GetWorld()->LineTraceSingleByChannel(OutHit, GetWorldCoordinatesFromPathfindingPoint(previousPoint), GetWorldCoordinatesFromPathfindingPoint(NextPoint), ECC_Visibility, QueryParams);
		if(condition4 && condition5 && !condition6) return true;
		return false;
	}
}

FInt32Vector UASAI_CharacterDetectionPathfinder::IndexToCoordinates(int32 index) const
{
	if (index < 0 || index >= m_dataAsset->m_Rows * m_dataAsset->m_Columns * m_dataAsset->m_Steps)
	{
		return FInt32Vector(-1, -1, -1); // Return an invalid point if the index is out of bounds
		
	}
	const int x = index / (m_dataAsset->m_Columns * m_dataAsset->m_Steps);
	const int y = (index % (m_dataAsset->m_Columns * m_dataAsset->m_Steps)) / m_dataAsset->m_Steps;
	const int z = index % m_dataAsset-> m_Steps;
	return FInt32Vector(x, y, z);
}

int32 UASAI_CharacterDetectionPathfinder::CoordinatesToIndex(FInt32Vector point) const
{
	if (point.X < 0 || point.X >= m_dataAsset->m_Rows ||
		   point.Y < 0 || point.Y >= m_dataAsset->m_Columns ||
		   point.Z < 0 || point.Z >= m_dataAsset->m_Steps) {
		return -1; // Return an invalid index if the point is out of bounds
		   }
	return point.X * (m_dataAsset->m_Columns * m_dataAsset->m_Steps) + point.Y * m_dataAsset->m_Steps + point.Z;
}

FVector UASAI_CharacterDetectionPathfinder::PathfindingCoordinatesToWorldCoordinates(FInt32Vector point) const
{
	float X = point.X * m_dataAsset->m_Spacing + m_dataAsset->m_OriginOffset.X;
	float Y = point.Y * m_dataAsset->m_Spacing + m_dataAsset->m_OriginOffset.Y;
	float Z = point.Z * m_dataAsset->m_Spacing + m_dataAsset->m_OriginOffset.Z;
	return FVector(X, Y, Z);
}



