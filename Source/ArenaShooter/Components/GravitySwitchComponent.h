// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravitySwitchComponent.generated.h"

class AASCharacter;
class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGravitySwitchComponent_Delegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARENASHOOTER_API UGravitySwitchComponent : public UActorComponent
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravitySwitchComponent|Settings", meta = (DisplayName = "Ability Cooldown"))
	float m_AbilityCooldown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravitySwitchComponent|Settings", meta = (DisplayName = "Gravity Max Nbr of Charge"))
	int m_MaxNbrOfCharge = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravitySwitchComponent|Settings", meta = (DisplayName = "Gravity Charge Refill Time"))
	float m_GravityChargeRefillTime = 10.f;
	
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "GravitySwitchComponent|Settings", meta = (DisplayName = "Is Switching Gravity"))
	bool bSwitchingGravity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GravitySwitchComponent|Debug", meta = (DisplayName = "Base Gravity Direction"))
	FVector m_BaseGravityDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GravitySwitchComponent|Settings", meta = (DisplayName = "Character"))
	AASCharacter* m_Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GravitySwitchComponent|Settings", meta = (DisplayName = "Gravity Nbr of Charge"))
	int m_NbrOfCharge = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GravitySwitchComponent|Settings", meta = (DisplayName = "Refill Timer"))
	float m_Timer = 0.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GravitySwitchComponent|Signs and Feedbacks")
	USoundCue* m_SoundOnGravitySwitch;
	
	FTimerHandle m_RefillTimerHandle;
	
public:
	UPROPERTY(BlueprintAssignable)
	FGravitySwitchComponent_Delegate OnStartSwitchGravity;
	
	UPROPERTY(BlueprintAssignable)
	FGravitySwitchComponent_Delegate OnSwitchGravityAbiltyCooldownEnd;

	UPROPERTY(BlueprintAssignable)
	FGravitySwitchComponent_Delegate OnGravityChargeRefill;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	UGravitySwitchComponent();
	
	UFUNCTION()
	void SwitchGravity();

	UFUNCTION()
	void RefullllGravityCharge();
	
	UFUNCTION()
	void RefullGravityCharge(int NbrToReset);
	
protected:
	virtual void BeginPlay() override;
	void EndSwitchGravity();
	void RefillGravityCharge();

public:
	FORCEINLINE bool IsSwitchingGravity() const { return bSwitchingGravity; }
	FORCEINLINE void SetSwitchingGravity(bool SwitchingGravity) { bSwitchingGravity = SwitchingGravity; }
	FORCEINLINE FVector GetBaseGravityDirection() const { return m_BaseGravityDirection; }
	FORCEINLINE float GetAbilityCooldown() const { return m_AbilityCooldown; }
	FORCEINLINE int GetNbrOfCharge() const { return m_NbrOfCharge; }
	FORCEINLINE float GetTimer() const { return m_Timer; }
	FORCEINLINE float GetGravityChargeRefillTime() const { return m_GravityChargeRefillTime; }
};
