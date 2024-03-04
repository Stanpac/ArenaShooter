// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaShooter/AI/ASPawn.h"
#include "GameFramework/Pawn.h"
#include "ASDronePawn.generated.h"

class UASHealthComponent;
class UASWeaponComponent;

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

public:
	AASDronePawn();

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "AI tick rate")
	float m_AITickRate;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Distance to trigger teleportation")
	float m_DistanceToTeleport;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Minimum distance to start shooting")
	float m_MinRangeToAttack;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Maximum distance to start shooting")
	float m_MaxRangeToAttack;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Radius of the detection sphere cast towards the player")
	float m_SphereCastRadius;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Time it takes to teleport through a wall")
	float m_TeleportationDuration;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Distance by sphere cast for teleportation")
	int32 m_DistanceByTeleportationSphereCast;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour", DisplayName = "Number of teleportation raycast tests before abort")
	int32 m_NumberOfTPBeforeAbort;
	
	UPROPERTY()
	float m_AITickTimer;

	UPROPERTY()
	AActor* m_ObjectHit;

	UPROPERTY()
	float m_DroneToPlayerMult;

	UPROPERTY()
	float m_TeleportationTimer;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ACharacter* m_Player;

	UPROPERTY()
	EDroneBehaviour m_CurrentBehaviour;
	
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
	void DroneMovement();
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
