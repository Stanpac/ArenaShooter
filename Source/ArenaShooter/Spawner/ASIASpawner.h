// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASIASpawner.generated.h"

class UBoxComponent;
class UArrowComponent;

UENUM()
enum ESpawnType
{
	VE_SpawnOnGround UMETA(DisplayName = "Spawn On Ground"),
	VE_SpawnInTheAir UMETA(DisplayName = "Spawn In The Air")
};

UENUM()
enum ESPawnZone
{
	VE_Box UMETA(DisplayName = "Box"),
	VE_Sphere UMETA(DisplayName = "Sphere"),
};

UCLASS()
class ARENASHOOTER_API AASIASpawner : public AActor
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:
	UPROPERTY(EditAnywhere, Category = "ASIASpawner", meta = (DisplayName = "AI Pawn Class To Spawn"))
	TSoftClassPtr<APawn> m_AIPawnClassTOSpawn;

	/* Type of this Spawner
	 * @SpawnOnGround : Spawn the AI on the ground with a raycast to check if the AI can be spawned
	 * @SpawnInTheAir : Spawn the AI in the air in the zone of spawn
	 */
	UPROPERTY(EditAnywhere, Category = "ASIASpawner", meta = (DisplayName = "Spawn Method"))
	TEnumAsByte<ESpawnType> m_SpawnType;

	UPROPERTY(EditAnywhere, Category = "ASIASpawner|", meta = (DisplayName = "Spawn Zone"))
	TEnumAsByte<ESPawnZone> m_SpawnZone;

	UPROPERTY(EditAnywhere, Category = "ASIASpawner|Settings", meta = (DisplayName = "Sphere Radius", EditCondition = "m_SpawnZone == ESPawnZone::VE_Sphere"))
	int m_SphereRadius;

	UPROPERTY(EditAnywhere, Category = "ASIASpawner|Settings", meta = (DisplayName = "Box Extend", EditCondition = "m_SpawnZone == ESPawnZone::VE_Box"))
	FVector m_BoxExtend;
	
	UPROPERTY(EditAnywhere, Category = "ASIASpawner|Settings", meta = (DisplayName = "Max Nbr Of AI Alive for this Spawner"))
	int m_SpawnedAIAliveMax;

	UPROPERTY(EditAnywhere, Category = "ASIASpawner|Settings", meta = (DisplayName = "Nbr Of AI Alive for this Spawner", ClampMin = 0))
	int m_SpawnedAIAliveCount;
	
	UPROPERTY(VisibleAnywhere)
	UShapeComponent* m_SpawnZoneComponent;

	UPROPERTY(VisibleAnywhere)
	TArray<APawn*> m_SpawnedAI;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate The Direction for the Raycast */
	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent;
#endif
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASIASpawner();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void Tick(float DeltaSeconds) override;

private:
	void ChangeSpawnZone();

	UFUNCTION()
	void OnAIDestroyed(AActor* DestroyedActor);
	
	void SpawnAI(FVector Location, FRotator Rotation);

	void SpawnAIOnGround();
	void SpawnAIInTheAir();
};

// Enum Method de Spawn de l'IA Sur le sol avec RayCast ou dans les airs
// Nbr d'IA En vie dans le niveau pour ce Spawner
// Zone De Spawn : Box, Sphere, Point
// Debug pour cette Zone de Spawn
// Button Dans l'inspector pour tester le spawn
// 