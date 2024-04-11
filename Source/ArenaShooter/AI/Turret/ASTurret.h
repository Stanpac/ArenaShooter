// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaShooter/AI/ASPawn.h"
#include "ASTurret.generated.h"

class UNiagaraComponent;
class UASTurretWidget;
class UBillboardComponent;
class USceneComponent;

UENUM(BlueprintType, Blueprintable)
enum class ETurretState : uint8
{
	ETS_Attack,
	ETS_Defence,
	ETS_Healing,
	ETS_Invalid
};

USTRUCT(BlueprintType, Blueprintable)
struct FTurretPhase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float m_BaseRotationSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	float m_RotationSpeedToAdd = 10.f;

	UPROPERTY(EditAnywhere)
	float m_TurrerHPMax = 100.f;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Time to reset take damage"))
	float m_TimeToResetTakeDamage = 1.f;
	
	FTimerHandle m_TakeDamageTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ASTurret|Settings", meta = (DisplayName = "Turret State"))
	ETurretState m_TurretState = ETurretState::ETS_Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Turret Laser Particule System"))
	UNiagaraComponent* m_TurretLaserParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Widget Class"))
	TSubclassOf<UASTurretWidget> m_WidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Widget"))
	UASTurretWidget* m_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Bump Force"))
	float m_BumpForce = 10.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTurret|Settings", meta = (DisplayName = "Turret Phases"))
	TArray<FTurretPhase> m_TurretPhases;

	UPROPERTY()
	int m_CurrentPhase = 0;

	FTimerHandle m_HealingTimerHandle;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASTurret(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer) override;

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION()
	void OnFire(FVector EndPosition);
	
protected:
	UFUNCTION()
	void EndTakeDamage();

	UFUNCTION()
	void ChangePhase();

	UFUNCTION()
	void Healing();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/* ---------------------------------- Get / Set --------------------------------------*/
public:
	
	FORCEINLINE ETurretState Get_TurretState() const { return m_TurretState; }
	FORCEINLINE void Set_TurretState(ETurretState state) { m_TurretState = state; }

	FORCEINLINE TObjectPtr<USceneComponent> GetRotationRootPoint() const { return m_RotationRootPoint; }
};
