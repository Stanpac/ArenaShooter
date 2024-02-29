// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Components/ASSpeedComponent.h"

#include "ArenaShooter/Character/ASCharacter.h"
#include "ArenaShooter/Data/SpeedProfilsData.h"
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
	m_EventWorldSubSystem = GetWorld()->GetSubsystem<UASEventWorldSubSystem>();
	m_EventWorldSubSystem->OnEnemyDeath.AddDynamic(this, &UASSpeedComponent::OnEnemyDeath);
	UpdateSpeedProfile(0);
}

void UASSpeedComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateSpeedBarValue( -DeltaTime * m_SpeedBarDecreasRate);
}

void UASSpeedComponent::UpdateSpeedProfile(int SpeedProfile)
{
	return;
	if(!IsValid(m_speedProfilData)) {
		return;
	}
	
	if (SpeedProfile < 0 || SpeedProfile >= m_speedProfilData->GetSpeedProfileCount()) {
		UE_LOG(LogTemp, Warning, TEXT("SpeedProfile out of range"));
		return;
	}
	
	FSpeedProfile profile = m_speedProfilData->GetSpeedProfile(SpeedProfile);
	m_CurrentSpeedProfile = profile.m_SpeedProfile;
	if (AASCharacter* character = CastChecked<AASCharacter>(GetOwner())) {
		if (UCharacterMovementComponent* MoveComp = character->GetCharacterMovement()) {
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

	OnUpdateSpeedProfile.Broadcast(m_CurrentSpeedProfile);
	m_EventWorldSubSystem->BroadcastSpeedProfileChange(m_CurrentSpeedProfile);
}

void UASSpeedComponent::UpdateSpeedBarValue(float Value)
{
	float temp = FMath::Clamp(m_SpeedBarValue + Value, 0.0f, m_SpeedBarMaxValue);
	if (temp >=  m_SpeedBarMaxValue) {
		UpdateSpeedProfile(m_CurrentSpeedProfile + 1);
		temp = m_SpeedBarMaxValue/2.0f;
	} else if (temp <= 0.0f) {
		UpdateSpeedProfile(m_CurrentSpeedProfile - 1);
		temp = m_SpeedBarMaxValue/2.0f;
	}
	m_SpeedBarValue = temp;
	OnUpdateSpeed.Broadcast(m_SpeedBarValue, m_SpeedBarMaxValue);
}

void UASSpeedComponent::UpdateSpeedBarMaxValue(float Value)
{
	m_SpeedBarMaxValue = Value;
}

void UASSpeedComponent::OnEnemyDeath()
{
	UpdateSpeedBarValue(m_EnemyDeathSpeedBarValueIncrease);
}






