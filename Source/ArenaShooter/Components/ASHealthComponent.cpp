// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Components/ASHealthComponent.h"	

#include "ArenaShooter/AI/ASPawn.h"
#include "ArenaShooter/Character/ASCharacter.h"
#include "ArenaShooter/Widget/ASGlobalWidget.h"
#include "Kismet/GameplayStatics.h"

UASHealthComponent::UASHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	m_Health = m_MaxHealth;
	m_IsExecutable = false;
}

void UASHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	m_Health = m_MaxHealth;
	if (!m_UseMultiplicator) {
		m_DamageMultiplicator = 1.0f;
		m_HealingMultiplicator = 1.0f;
	}
	
	OnHealthChanged.Broadcast(0, m_Health, m_MaxHealth, GetOwner());
}

void UASHealthComponent::healing(float amount)
{
	// previous health can be Use Later For Lerp maybe ?
	float PreviousHealth = m_Health;
	float NewHealth = FMath::Clamp(PreviousHealth + (amount * m_HealingMultiplicator), m_IsInvincible ? 1 : 0, m_MaxHealth);
	
	m_Health = NewHealth;
	OnHealthChanged.Broadcast(PreviousHealth, m_Health, m_MaxHealth, GetOwner());
}

void UASHealthComponent::Damage(float amount, AActor* DamageDealer, float stunDuration, FVector hitLocation, FRotator hitRotation)
{
	if(!m_IsDamageable) return;

	if(GetOwner()->IsA<AASPawn>())
	{
		AASPawn* pawn = Cast<AASPawn>(GetOwner());
		if(hitLocation == FVector::ZeroVector)
		{
			hitLocation = pawn->GetActorLocation();
		}
		pawn->SetHitPosition(hitLocation);
	}

	// previous health can be Use Later For Lerp maybe ?
	const float PreviousHealth = m_Health;
	const float NewHealth = FMath::Clamp(PreviousHealth - (amount * m_DamageMultiplicator), m_IsInvincible ? 1 : 0, m_MaxHealth);

	if(GetOwner()->IsA<AASPawn>()) {
		Cast<AASPawn>(GetOwner())->Stun(stunDuration);
	}
	
	m_Health = NewHealth;
	
	if(m_Health <= m_ExecutableLife && !m_IsExecutable) {
		m_IsExecutable = true;
	}
	
	OnHealthChanged.Broadcast(PreviousHealth, m_Health, m_MaxHealth, DamageDealer);
	
	if (m_Health <= 0.0f) {
		OnDeathStarted.Broadcast(GetOwner());
	}
}



