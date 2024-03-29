// Fill out your copyright notice in the Description page of Project Settings.


#include "GravitySwitchComponent.h"

#include "ArenaShooter/Character/ASCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// TODO : Fix Gravity image visibility problm

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
    if (IsSwitchingGravity() || !IsValid(m_Character) || m_NbrOfCharge <=0 ) return;
    m_Character->GetCharacterMovement()->SetGravityDirection(-m_Character->GetGravityDirection());
	OnStartSwitchGravity.Broadcast();
	SetSwitchingGravity(true);
	m_NbrOfCharge--;

	if (!GetWorld()->GetTimerManager().IsTimerActive(m_RefillTimerHandle)) {
		GetWorld()->GetTimerManager().SetTimer(m_RefillTimerHandle, this, &UGravitySwitchComponent::RefillGravityCharge,0.1f, true);
	}
	
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGravitySwitchComponent::EndSwitchGravity,m_AbilityCooldown, false);
}

void UGravitySwitchComponent::RefullllGravityCharge()
{
	m_NbrOfCharge = m_MaxNbrOfCharge;
}

void UGravitySwitchComponent::RefullGravityCharge(int NbrToReset)
{
	m_NbrOfCharge = FMath::Clamp(NbrToReset, 0, m_MaxNbrOfCharge);
}

void UGravitySwitchComponent::EndSwitchGravity()
{
	SetSwitchingGravity(false);
	OnSwitchGravityAbiltyCooldownEnd.Broadcast();
}


void UGravitySwitchComponent::RefillGravityCharge()
{
	OnGravityChargeRefill.Broadcast();
	
	if (m_NbrOfCharge >= m_MaxNbrOfCharge) {
		GetWorld()->GetTimerManager().ClearTimer(m_RefillTimerHandle);
		m_Timer = 0.f;
		return;
	}
	
	m_Timer += 0.1f;
	if (m_Timer >= m_GravityChargeRefillTime) {
		m_Timer = 0.f;
		m_NbrOfCharge = FMath::Clamp(m_NbrOfCharge + 1, 0, m_MaxNbrOfCharge);
	}
}
