// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ASPawn.generated.h"

class USphereComponent;
class UCapsuleComponent;
class UASHealthComponent;
class UStaticMeshComponent;
class UWidgetComponent;
class UUserWidget;
class UASEventWorldSubSystem;
class UASWeaponComponent;
class USceneComponent;
class UNiagaraSystem;


UCLASS()
class ARENASHOOTER_API AASPawn : public APawn
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "RootComponent"))
	USceneComponent* m_RootComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "MeshComponent"))
	UStaticMeshComponent* m_MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "CapsuleComponent"))
	UCapsuleComponent* m_CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "HealthComponent"))
	UASHealthComponent* m_HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "WeaponComponent"))
	UASWeaponComponent* m_WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "Death parrticle"))
	UParticleSystem* m_DeathParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "Impact parrticle"));
	UParticleSystem* m_ImpactParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ASPawn|Damage", meta = (AllowPrivateAccess = "true", DisplayName = "FloatingDamageSystem"))
	TObjectPtr<UNiagaraSystem> m_floatingDamageSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPawn|Settings", meta = (DisplayName = "Indicator Widget Class"))
	TSubclassOf<UUserWidget> m_IndicatorWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPawn|Settings", meta = (DisplayName = "Indicator base Color"))
	FColor m_IndicatorBaseColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPawn|Settings", meta = (DisplayName = "Indicator Color When Shooting"))
	FColor m_IndicatorShootingColor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "Indicator Widget"))
	UUserWidget* m_IndicatorWidget;


	/** How much time is left to end stun **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stun", meta=(DisplayName = "Current Stun Timer"))
	float m_StunTimer;
	
	/** Is the pawn stunned **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stun", meta=(DisplayName = "Is Stunned ?"))
	bool m_IsStunned;

	UPROPERTY(EditDefaultsOnly, Category = "Signs and Feedbacks", DisplayName = "Lock in sound")
	USoundBase* m_LockInSound;
	
	/** Event World SubSystem */
	UPROPERTY()
	TObjectPtr<UASEventWorldSubSystem> m_EventWorldSubSystem;

	UPROPERTY()
	FVector m_HitLocation;

	UPROPERTY()
	FRotator m_HitRotation;
	
	// Timer
	FTimerHandle m_HealthBarTimerHandle;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void Stun(float stunDuration) ;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	virtual void OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer);

	UFUNCTION()
	virtual void OnDeath(AActor* DeathDealer);

	UFUNCTION()
	virtual void StunTick(float DeltaTime);

	UFUNCTION()
	void ManageIndicatorWidget();
	
	UFUNCTION(BlueprintCallable)
	void SpawnFloatingDamage(const FVector& SpawnLocation, const FRotator& SpawnRotation, const float Damage);

public:
	bool GetIsStunned() const { return m_IsStunned;}
	void SetHitPosition(const FVector& hitPosition);
	void SetHitRotation(const FRotator& hitRotation);

};
