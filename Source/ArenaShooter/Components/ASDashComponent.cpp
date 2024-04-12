#include "ASDashComponent.h"

#include "ASHealthComponent.h"
#include "GravitySwitchComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ArenaShooter/AI/ASPawn.h"
#include "ArenaShooter/Character/ASCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"


UASDashComponent::UASDashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UASDashComponent::BeginPlay()
{
	Super::BeginPlay();
	m_Character = Cast<AASCharacter>(GetOwner());
	m_StartGravity = m_Character->GetCharacterMovement()->GravityScale;
	m_StartGroundFriction = m_Character->GetCharacterMovement()->GroundFriction;
	m_Camera = m_Character->GetCameraComponent();
	m_BaseFieldOfView = m_Camera->FieldOfView;
	m_DashDamage = m_BaseDashDamage;
}

void UASDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch(m_CurrentDashState)
	{
		case EDashStates::InDash:
			DashMovement(DeltaTime);
			break;
		case EDashStates::NoGravity:
			m_NoGravityTimer -= DeltaTime;
			if(m_NoGravityTimer <= 0)
			{
				m_CurrentDashState = EDashStates::Neutral;
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
				m_Character->GetCharacterMovement()->GravityScale = m_StartGravity;
				m_Character->GetCharacterMovement()->GroundFriction = m_StartGroundFriction;
				m_Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				UASHealthComponent::FindHealthComponent(GetOwner())->SetIsDamageable(true);
			}
			break;
		case EDashStates::InCooldown:
			m_DashCooldownTimer -= DeltaTime;
			m_Character->OnDashRechargeTick(1 - m_DashCooldownTimer / m_DashCooldown);
			if(m_DashCooldownTimer <= 0)
			{
				m_CurrentDashState = EDashStates::Neutral;
				m_Character->OnDashAbilityCooldownEnd();
				if(IsValid(m_SoundDashAvailable))
				{
					UGameplayStatics::PlaySoundAtLocation(
						GetWorld(),
						m_SoundDashAvailable,
						GetOwner()->GetActorLocation());
				}
			}
			break;
		case EDashStates::Neutral:
			break;
	}
	
	m_TickRefreshTimer -= DeltaTime;
	if(m_TickRefreshTimer <= 0)
	{
		SlowTick(DeltaTime);
		m_TickRefreshTimer = m_TickRefreshRate;
	}
	OnHitTargetChange.Broadcast(m_HitTarget);
}

bool UASDashComponent::OnDash()
{
	if(m_CurrentDashState != EDashStates::Neutral && m_CurrentDashState != EDashStates::NoGravity) return false;

	if(m_CurrentDashState != EDashStates::NoGravity)
		m_Inertia = m_Character->GetCharacterMovement()->Velocity;
	m_HitTarget = DetectDashTarget();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
	if(IsValid(m_SoundDash))
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			m_SoundDash,
			GetOwner()->GetActorLocation());
	}
	m_CurrentDashState = EDashStates::InDash;
	m_DashDurationTimer = 0;
	m_DashStartLocation = GetOwner()->GetActorLocation();
	m_Character->GetCharacterMovement()->GroundFriction = 0;
	m_Character->GetCharacterMovement()->MovementMode = MOVE_Flying;
	m_Character->GetCharacterMovement()->DisableMovement();
	m_Character->OnDashValidate();
	UASHealthComponent::FindHealthComponent(GetOwner())->SetIsDamageable(false);
	if(m_HitTarget == nullptr)
	{
		m_DashEndLocation = m_DashStartLocation + m_Camera->GetForwardVector() * m_DashDistance;
		return false;
	}
	else
	{
		if(UASHealthComponent::FindHealthComponent(m_HitTarget)->GetIsExecutable())
		{
			m_DashEndLocation = m_HitTarget->GetActorLocation() + (m_HitTarget->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal() * m_DashOffset;
			m_HitTarget->SetActorEnableCollision(false);
		}
		else
		{
			m_DashEndLocation = m_HitTarget->GetActorLocation() + (m_HitTarget->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal() * -m_DashOffset;
		}
		return true;
	}
}

void UASDashComponent::SlowTick(float DeltaTime)
{
	if(m_CurrentDashState != EDashStates::InDash)
	{
		m_HitTarget = DetectDashTarget();
	}
}

void UASDashComponent::DashMovement(float DeltaTime)
{
	m_DashDurationTimer += DeltaTime / m_DashDuration;
	GetOwner()->SetActorLocation(FMath::Lerp(m_DashStartLocation, m_DashEndLocation, m_DashSpeedCurve->GetFloatValue(m_DashDurationTimer)), true);
	m_Camera->FieldOfView = FMath::Lerp(m_BaseFieldOfView, m_MaxFieldOfView, m_FOVevolutionCurve->GetFloatValue(m_DashDurationTimer));

	if(m_DashDurationTimer >= 1)
	{
		m_Camera->FieldOfView = m_BaseFieldOfView;
		if(IsValid(m_HitTarget))
		{
			GetOwner()->SetActorLocation(m_DashEndLocation, false);
			UASHealthComponent* HealthComponent = UASHealthComponent::FindHealthComponent(m_HitTarget);
			if(HealthComponent != nullptr)
			{
				if(HealthComponent->GetIsExecutable())
				{
					HealthComponent->Damage(m_DashDamage, GetOwner(), m_StunDuration);
					m_CurrentDashState = EDashStates::NoGravity;
					UASHealthComponent::FindHealthComponent(m_Character)->healing(3);
					m_NoGravityTimer = m_NoGravityCooldown;
					if(IsValid(m_SoundDash))
					{
						UGameplayStatics::PlaySoundAtLocation(
							GetWorld(),
							m_SoundDashAvailable,
							GetOwner()->GetActorLocation());
					}
					m_Character->OnDashAbilityCooldownEnd();
					if(m_Character->m_GravitySwitchComponent->m_IsInSurtension)
					{
						m_Character->m_GravitySwitchComponent->m_SurtensionTimer += m_SurtensionExtensionDuration;
						m_Character->m_GravitySwitchComponent->m_SurtensionTimer = FMath::Max(m_Character->m_GravitySwitchComponent->m_SurtensionTimer, m_Character->m_GravitySwitchComponent->m_SurtensionDuration);
					}
					UGameplayStatics::SetGlobalTimeDilation(GetWorld(), m_TimeSlowValue);
					return;
				}
				else
				{
					HealthComponent->Damage(m_DashDamage, GetOwner(), m_StunDuration);
				}
			}
		}
		else
		{
			GetOwner()->SetActorLocation(m_DashEndLocation, true);
			m_Character->GetCharacterMovement()->Velocity = m_Inertia;
		}
		UASHealthComponent::FindHealthComponent(GetOwner())->SetIsDamageable(true);
		m_Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		m_CurrentDashState = EDashStates::InCooldown;
		//m_Character->GetCharacterMovement()->GravityScale = m_StartGravity;
		m_Character->GetCharacterMovement()->GroundFriction = m_StartGroundFriction;
		m_DashCooldownTimer = m_DashCooldown;
		m_Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		UASHealthComponent::FindHealthComponent(GetOwner())->SetIsDamageable(true);
	}
}

AActor* UASDashComponent::DetectDashTarget()
{
	FColor debugColor = m_HitTarget == nullptr ? FColor::Black : FColor::White;
	m_HitTarget = nullptr;
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore {GetOwner()};
	
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = StartLocation + m_Camera->GetForwardVector() * m_DashTargetDetectionRange;
	
	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2);
	ETraceTypeQuery TraceTypeQueryToValidate = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3);
	
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
	GetWorld(),
	StartLocation,
	EndLocation,
	m_DashTargetDetectionRadius,
	TraceTypeQuery,
	false,
	ActorsToIgnore,
	EDrawDebugTrace::None,
	OutHits,
	true,
	debugColor);
	
	if(bHit)
	{
		for(auto hit : OutHits)
		{
			FVector direction = (EndLocation - StartLocation).GetSafeNormal() * 100;
			EndLocation = hit.GetActor()->GetActorLocation() + direction;
			FHitResult OutHit;
			
			bool bHitValidation = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			StartLocation,
			EndLocation,
			TraceTypeQueryToValidate,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			OutHit,
			true);
			if(bHitValidation)
			{
				if(OutHit.GetActor()->IsA<AASPawn>())
				{
					//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Black, OutHit.GetActor()->GetName());
					FRotator rot = (GetOwner()->GetActorLocation() - OutHit.Location).Rotation();
					DrawDebugCrosshairs(GetWorld(), OutHit.GetActor()->GetActorLocation(), rot, 100, FColor::Red, false, m_TickRefreshRate);
					return OutHit.GetActor();;
				}
			}
		}
	}

	return nullptr;
}

