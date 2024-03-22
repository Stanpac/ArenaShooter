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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "MeshComponent"))
	UStaticMeshComponent* m_MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "CapsuleComponent"))
	UCapsuleComponent* m_CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "HealthComponent"))
	UASHealthComponent* m_HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "WeaponComponent"))
	UASWeaponComponent* m_WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "HealthVisibilitySphereComponent"))
	USphereComponent* m_HealthVisibilitySphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASPawn|Components", meta = (DisplayName = "HealthBarWidgetComponent"))
	UWidgetComponent* m_HealthBarWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASPawn|Widget", meta = (DisplayName = "HealthBarWidgetClass"))
	TSubclassOf<UUserWidget> m_HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASPawn|Temp", meta = (DisplayName = "Copy"))
	TSubclassOf<AASPawn> m_TurretCopy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "Death parrticle"))
	UParticleSystem* m_DeathParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "Impact parrticle"));
	UParticleSystem* m_ImpactParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ASPawn|Damage", meta = (AllowPrivateAccess = "true", DisplayName = "FloatingDamageSystem"))
	TObjectPtr<UNiagaraSystem> m_floatingDamageSystem;

	/** How much time is left to end stun **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stun", meta=(DisplayName = "Current Stun Timer"))
	float m_StunTimer;
	
	/** Is the pawn stunned **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stun", meta=(DisplayName = "Is Stunned ?"))
	bool m_IsStunned;
	
	/** Event World SubSystem */
	UPROPERTY()
	TObjectPtr<UASEventWorldSubSystem> m_EventWorldSubSystem;
	
	// Timer
	FTimerHandle m_HealthBarTimerHandle;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASPawn();

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void Stun(float stunDuration) ;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	void OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer);

	UFUNCTION()
	virtual void OnDeath(AActor* DeathDealer);

	UFUNCTION()
	virtual void StunTick(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void SpawnFloatingDamage(const FVector& SpawnLocation, const FRotator& SpawnRotation, const float Damage);

public:
	
	bool GetIsStunned() const { return m_IsStunned;}

protected:
	void SetWidgetVisibility(bool visible);

	void SetWidgetVisibilityfalse();

	UFUNCTION()
	virtual void OnHealthVisibilitySphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnHealthVisibilitySphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);
	
};
