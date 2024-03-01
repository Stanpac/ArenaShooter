// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravitySwitchComponent.generated.h"

class AASCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGravitySwitchComponent_Delegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARENASHOOTER_API UGravitySwitchComponent : public UActorComponent
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravitySwitchComponent|Settings", meta = (DisplayName = "Ability Cooldown"))
	float m_AbilityCooldown = 5.0f;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "GravitySwitchComponent|Settings", meta = (DisplayName = "Is Switching Gravity"))
	bool bSwitchingGravity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GravitySwitchComponent|Debug", meta = (DisplayName = "Base Gravity Direction"))
	FVector m_BaseGravityDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GravitySwitchComponent|Settings", meta = (DisplayName = "Character"))
	AASCharacter* m_Character;
	
public:
	UPROPERTY(BlueprintAssignable)
	FGravitySwitchComponent_Delegate OnSwitchGravity;
	
	UPROPERTY(BlueprintAssignable)
	FGravitySwitchComponent_Delegate OnSwitchGravityAbiltyCooldownEnd;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	UGravitySwitchComponent();

	void SwitchGravity();
	

protected:
	virtual void BeginPlay() override;

	void EndSwitchGravity();

public:
	FORCEINLINE bool IsSwitchingGravity() const { return bSwitchingGravity; }
	FVector GetBaseGravityDirection() const { return m_BaseGravityDirection; }
};
