// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASHealthComponent.generated.h"

class AASCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthComponent_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHealthComponent_HealthChangeEvent, float, Oldvalue, float, NewValue, float, MaxHealth, AActor*, DamageDealer);

/**
 * Actor Component that manage the health of an actor
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENASHOOTER_API UASHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ASHealthComponent|Health")
	float m_Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASHealthComponent|Health")
	float m_MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASHealthComponent|Healing")
	uint8 m_UseMultiplicator : 1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASHealthComponent|Settings", meta = (DisplayName = "Is Invincible"))
	uint8 m_IsInvincible : 1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASHealthComponent|Settings", meta = (DisplayName = "Executable life"))
	float m_ExecutableLife = 15.f;
	
	/** Multiplicative For the healing
	 *  @note 1.0f = 100% of the healing is applied
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASHealthComponent|Healing|Multiplicator", meta = (EditCondition = "m_UseMultiplicator"))
	float m_HealingMultiplicator;

	/** Multiplicative For the Damage
	 *  @note 1.0f = 100% of the healing is applied
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASHealthComponent|Healing|Multiplicator", meta = (EditCondition = "m_UseMultiplicator"))
	float m_DamageMultiplicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASHealthComponent|Health", meta = (DisplayName = "Is Executatble"))
	bool m_IsExecutable;
	
	UPROPERTY()
	bool m_IsDamageable = true;

public:
	// Delegate call when the Death Start
	UPROPERTY(BlueprintAssignable)
	FHealthComponent_DeathEvent OnDeathStarted;
	
	// Delegate call when the Health Change
	UPROPERTY(BlueprintAssignable)
	FHealthComponent_HealthChangeEvent OnHealthChanged;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	UASHealthComponent();

	UFUNCTION(BlueprintPure, Category = "ASHealthComponent|Health")
	static UASHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UASHealthComponent>() : nullptr) ;}

	UFUNCTION(BlueprintCallable)
	void healing(float amount);
	
	UFUNCTION(BlueprintCallable)
	void Damage(float amount, AActor* DamageDealer, float stunAmount = 0, FVector hitLocation = FVector::ZeroVector, FRotator hitRotation = FRotator::ZeroRotator);

	
protected:
	virtual void BeginPlay() override;

public:
	bool GetIsDamageable() const { return m_IsDamageable; }
	void SetIsDamageable(bool isDamageable) { m_IsDamageable = isDamageable;}

	FORCEINLINE float GetHealth() const { return m_Health; }
	FORCEINLINE float GetMaxHealth() const { return m_MaxHealth; }
	FORCEINLINE bool GetIsExecutable() const { return m_IsExecutable; }
	FORCEINLINE void SetIsExecutable(bool bM_IsExecutable) { m_IsExecutable = bM_IsExecutable; }
};
