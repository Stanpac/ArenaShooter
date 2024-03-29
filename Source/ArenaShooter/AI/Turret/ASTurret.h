// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaShooter/AI/ASPawn.h"
#include "ASTurret.generated.h"

UCLASS()
class ARENASHOOTER_API AASTurret : public AASPawn
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Turret Range"))
	float m_TurretRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Turret Rotation Speed"))
	float m_TurretRotationSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Shooting point"))
	TObjectPtr<USceneComponent> m_RotationRootPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Moving mesh component"))
	TObjectPtr<UStaticMeshComponent> m_MovingMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Shooting point"))
	TObjectPtr<USceneComponent> m_ShootingPoint;
	
	// Put With editor Only
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Debug Shoot point"))
	TObjectPtr<UBillboardComponent> m_DebugShootPoint;
	// End Put With editor Only

	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASTurret(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	virtual void OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer) override;


};
