// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaShooter/AI/ASPawn.h"
#include "GameFramework/Pawn.h"
#include "ASDronePawn.generated.h"

class UASHealthComponent;
class UASWeaponComponent;
class UASDroneManager;

UENUM(BlueprintType) // Makes the enum available in Blueprints
enum class EDroneBehaviour : uint8
{
	CHASING UMETA(DisplayName = "Chasing"),
	SHOOTING UMETA(DisplayName = "Shooting"),
	TELEPORTING UMETA(DisplayName = "Teleporting")
};

UCLASS()
class ARENASHOOTER_API AASDronePawn : public AASPawn
{
	GENERATED_BODY()

private:
	/*---------------------------------- MEMBERS --------------------------------------*/

	UPROPERTY(EditAnywhere, Category = "AI Enablers", DisplayName = "Enable shooting")
	bool m_EnableShooting = true;

	UPROPERTY(EditAnywhere, Category = "AI Enablers", DisplayName = "Enable movement towards player")
	bool m_EnableMovementTowardsPlayer = true;

	UPROPERTY(EditAnywhere, Category = "AI Enablers", DisplayName = "Enable dispersion behaviour")
	bool m_EnableDispersionBehaviour = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "AI tick rate")
	float m_AITickRate = .25f;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Distance to trigger teleportation")
	float m_DistanceToTeleport = 50;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Minimum distance to start shooting")
	float m_MinRangeToAttack = 600;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Maximum distance to start shooting")
	float m_MaxRangeToAttack = 1000;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Radius of the detection sphere cast towards the player")
	float m_SphereCastRadius = 20;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Time it takes to teleport through a wall")
	float m_TeleportationDuration = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Distance by sphere cast for teleportation")
	int32 m_DistanceByTeleportationSphereCast = 100;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Number of teleportation raycast tests before abort")
	int32 m_NumberOfTPBeforeAbort = 30;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Speed of drone towards Player")
	float m_DroneToPlayerSpeed = 100;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Speed of drone away from Player")
	float m_DroneAwayFromPlayerSpeed = 100;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Speed of drone away from other drones")
	float m_DroneDispersionSpeed = 200;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Curve of dispersion from other drones depending on distance")
	UCurveFloat* m_DroneDispersionCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Max Variance of the factor on the dispersion Value")
	float m_DispersionFactorMax = 1.2f;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Min Variance of the factor on the dispersion Value")
	float m_DispersionFactorMin = .8f;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Curve of evolution of speed evaluated by distance")
	UCurveFloat* m_DistanceSpeedCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Speed multiplier to the distance between player and drone")
	float m_DistanceSpeedMult = 100;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Max distance evaluated for the speed by distance multiplier")
	float m_MaxDistanceEvaluated = 5000;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Time to lock on target")
	float m_TimeToLockInTarget = 1;
	
	UPROPERTY()
	float m_AITickTimer;

	UPROPERTY()
	AActor* m_ObjectHit;

	UPROPERTY()
	float m_DroneToPlayerMult;

	UPROPERTY()
	float m_TeleportationTimer;

	UPROPERTY()
	UASDroneManager* m_DroneManager;

	UPROPERTY()
	FVector m_DispersionVector;

	UPROPERTY()
	float m_DispersionFactor;

protected:
	/** Player Reference */
	UPROPERTY()
	ACharacter* m_Player;

	UPROPERTY()
	EDroneBehaviour m_CurrentBehaviour;

	/*---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASDronePawn();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void LookAtPlayer(float DeltaTime);

	UFUNCTION()
	void ChangeBehaviour(EDroneBehaviour newBehaviour);

	UFUNCTION()
	void OnBehaviourStateEnter();

	UFUNCTION()
	void OnBehaviourStateExit();

	UFUNCTION()
	void OnBehaviourStateTick(float DeltaTime);

	UFUNCTION()
	void OnBehaviourAIStateTick();

	UFUNCTION()
	void ChasingBehaviour();
	
	UFUNCTION()
	bool IsPlayerInSight(float& DistanceToTarget);

	UFUNCTION()
	void TeleportDrone();
	
	UFUNCTION()
	void DroneMovement(float DeltaTime);

	UFUNCTION()
	FVector DroneRelativeMoveDirection();
	
public:	
	virtual void Tick(float DeltaTime) override;
};
