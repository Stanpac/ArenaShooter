// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaShooter/AI/ASPawn.h"
#include "ASTurret.generated.h"

class UNiagaraComponent;

UENUM(BlueprintType, Blueprintable)
enum class ETurretState : uint8
{
	ETS_Attack,
	ETS_Defence,
	ETS_Invalid
};

UCLASS()
class ARENASHOOTER_API AASTurret : public AASPawn
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTurret|Component", meta = (DisplayName = "Rotation root point"))
	TObjectPtr<USceneComponent> m_RotationRootPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Component", meta = (DisplayName = "Moving mesh component"))
	TObjectPtr<UStaticMeshComponent> m_MovingMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Component", meta = (DisplayName = "Shooting point"))
	TObjectPtr<USceneComponent> m_ShootingPoint;
	
	// Put With editor Only
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASTurret|Component", meta = (DisplayName = "Debug Shoot point"))
	TObjectPtr<UBillboardComponent> m_DebugShootPoint;
	// End Put With editor Only

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Turret Range"))
	float m_TurretRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Turret Rotation Speed"))
	float m_TurretRotationSpeed = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Have Take damage recently"))
	bool m_HaveTakeDamageRecently = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Time to reset take damage"))
	float m_TimeToResetTakeDamage = 1.f;
	
	FTimerHandle m_TakeDamageTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ASTurret|Settings", meta = (DisplayName = "Turret State"))
	ETurretState m_TurretState = ETurretState::ETS_Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Turret Laser Particule System"))
	UNiagaraComponent* m_TurretLaserParticleSystem;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASTurret(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer) override;

	UFUNCTION(BlueprintCallable)
	void Shoot();

protected:
	UFUNCTION()
	void ResetTakeDamage();
	
	/* ---------------------------------- Get / Set --------------------------------------*/
public:
	FORCEINLINE bool Get_HaveTakeDamageRecently() const { return m_HaveTakeDamageRecently; }
	
	FORCEINLINE ETurretState Get_TurretState() const { return m_TurretState; }
	FORCEINLINE void Set_TurretState(ETurretState state) { m_TurretState = state; }

	FORCEINLINE TObjectPtr<USceneComponent> GetM_RotationRootPoint() const { return m_RotationRootPoint; }
};
