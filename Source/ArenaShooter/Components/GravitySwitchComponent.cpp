// Fill out your copyright notice in the Description page of Project Settings.


#include "GravitySwitchComponent.h"

#include "ArenaShooter/Character/ASCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


UGravitySwitchComponent::UGravitySwitchComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGravitySwitchComponent::BeginPlay()
{
	Super::BeginPlay();
	m_Character = CastChecked<AASCharacter>(GetOwner());
	m_BaseGravityDirection = m_Character->GetGravityDirection();
}

void UGravitySwitchComponent::SwitchGravity()
{
    if (bSwitchingGravity || !IsValid(m_Character)) return;
	OnSwitchGravity.Broadcast();
    bSwitchingGravity = true;
    m_Character->GetCharacterMovement()->SetGravityDirection(-m_Character->GetGravityDirection());
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGravitySwitchComponent::EndSwitchGravity,m_AbilityCooldown , false);
}

void UGravitySwitchComponent::EndSwitchGravity()
{
	bSwitchingGravity = false;
	OnSwitchGravityAbiltyCooldownEnd.Broadcast();
}
