// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Components/ASSpeedComponent.h"

#include "ArenaShooter/Character/ASCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UASSpeedComponent::UASSpeedComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UASSpeedComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdateSpeedProfile(0);
}

void UASSpeedComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateSpeedBarValue( -DeltaTime * m_SpeedBarDecreasRate);
}

void UASSpeedComponent::UpdateSpeedProfile(int SpeedProfile)
{
	if (SpeedProfile < 0 || SpeedProfile >= m_SpeedProfile.Num()) {
		UE_LOG(LogTemp, Warning, TEXT("SpeedProfile out of range"));
		return;
	}
	
	FSpeedProfile profile = m_SpeedProfile[SpeedProfile];
	m_CurrentSpeedProfile = profile.m_SpeedProfile;
	if (AASCharacter* character = CastChecked<AASCharacter>(GetOwner())) {
		UCharacterMovementComponent* MoveComp = character->GetCharacterMovement();
		MoveComp->MaxAcceleration = profile.m_MaxAcceleration;
		MoveComp->MaxWalkSpeed = profile.m_MaxWalkSpeed;
		MoveComp->Mass = profile.m_Mass;
		MoveComp->GravityScale = profile.m_Gravity;
		MoveComp->JumpZVelocity = profile.m_JumpZ;
		MoveComp->AirControl = profile.m_AirControl;
		MoveComp->BrakingDecelerationFalling = profile.m_BreakingDecelerationFalling;
		UpdateSpeedBarMaxValue(profile.m_SpeedBarValueMax);
		m_SpeedBarDecreasRate = profile.m_SpeedBarDecreasRate;
	}
}

void UASSpeedComponent::UpdateSpeedBarValue(float Value)
{
	float temp = FMath::Clamp(m_SpeedBarValue + Value, 0.0f, m_SpeedBarMaxValue);
	if (temp >=  m_SpeedBarMaxValue) {
		temp = 0.0f;
		UpdateSpeedProfile(m_CurrentSpeedProfile + 1);
	} else if (temp <= 0.0f) {
		UpdateSpeedProfile(m_CurrentSpeedProfile - 1);
		temp = m_SpeedBarMaxValue;
	}
	m_SpeedBarValue = temp;
}

void UASSpeedComponent::UpdateSpeedBarMaxValue(float Value)
{
	m_SpeedBarMaxValue = Value;
}






