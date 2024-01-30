// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Components/ASHealthComponent.h"	

#include "ArenaShooter/Character/ASCharacter.h"
#include "ArenaShooter/Widget/ASGlobalWidget.h"

UASHealthComponent::UASHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	m_Health = m_MaxHealth;
}

void UASHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	m_Health = m_MaxHealth;
	if (!m_UseMultiplicator) {
		m_DamageMultiplicator = 1.0f;
		m_HealingMultiplicator = 1.0f;
	}
	
	UpdateWidget();
}

void UASHealthComponent::UpdateWidget()
{
	// TODO : Change this to have a Widget Comp on Charcter, this way player abd AI can have a widget
	if (AASCharacter* Character = Cast<AASCharacter>(GetOwner())) {
		if (UASGlobalWidget* Widget = Character->GetPlayerWidget()) {
			Widget->UpdatehealthBar(m_Health / m_MaxHealth);
		}
	}
}

void UASHealthComponent::healing(float amount)
{
	// previous health can be Use Later For Lerp maybe ?
	float PreviousHealth = m_Health;
	float NewHealth = PreviousHealth + (amount * m_HealingMultiplicator);
	
	if (NewHealth > m_MaxHealth) {
		NewHealth = m_MaxHealth;
	}
	
	m_Health = NewHealth;
	UpdateWidget();
}

void UASHealthComponent::Damage(float amount)
{
	// previous health can be Use Later For Lerp maybe ?
	float PreviousHealth = m_Health;
	float NewHealth = PreviousHealth - (amount * m_DamageMultiplicator);

	if (NewHealth < 0.0f) {
		NewHealth = 0.0f;
	}

	m_Health = NewHealth;
	UpdateWidget();
}


