// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASCloseCombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARENASHOOTER_API UASCloseCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UASCloseCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void CooldownTick(float DeltaTime);

	virtual void CooldownTrigger();
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void StartCloseCombatAttack();
	
	/** How much damage is dealt to any touched target with a healthcomponent **/
	UPROPERTY(EditAnywhere, Category="Stats", meta = (DisplayName="Weapon Damage"))
	float m_Damage;

	/** How much time before the player is able to do a close combat attack again **/
	UPROPERTY(EditAnywhere, Category="Stats", meta = (DisplayName="Cooldown Duration"))
	float m_CooldownDuration;

	/** How much time before the player can attack again **/
	UPROPERTY(EditAnywhere, Category="Stats", meta = (DisplayName="Cooldown Timer"))
	float m_CooldownTimer;

	/** Can the player perform a close combat attack **/
	UPROPERTY(EditAnywhere, Category="Stats", meta = (DisplayName="Can Attack?"))
	bool m_CanAttack;
	
	
	/** Size of the sphere in which damage will be triggered **/
	UPROPERTY(EditAnywhere, Category="Stats", meta = (DisplayName="Attack Zone Radius"))
	float m_DamageZoneRadius;

	/** Distance from the player where the collision sphere for the damage will be casted **/
	UPROPERTY(EditAnywhere, Category="Stats", meta = (DisplayName="Attack Range"))
	float m_AttackRange;

	/** Stun Duration When Ennemy is Hit **/
	UPROPERTY(EditAnywhere, Category="Stats", meta = (DisplayName="Stun Duration"))
	float m_StunDuration;
	
	/** Sound Played when performing a missed close combat attack**/
	UPROPERTY(EditAnywhere, Category="Feedbacks", meta = (DisplayName="Missed Attack Sound"))
	USoundBase* m_Sound_MissedCloseCombatAttack;

	/** Sound Played when performing a successful close combat attack**/
	UPROPERTY(EditAnywhere, Category="Feedbacks", meta = (DisplayName="Hit Attack Sound"))
	USoundBase* m_Sound_SuccessfulCloseCombatAttack;
};
