#include "ASCloseCombatComponent.h"
#include "ASHealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UASCloseCombatComponent::UASCloseCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UASCloseCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UASCloseCombatComponent::CooldownTick(float DeltaTime)
{
	m_CooldownTimer -= DeltaTime;
	if(m_CooldownTimer < 0)
	{
		m_CanAttack = true;
	}
}

void UASCloseCombatComponent::CooldownTrigger()
{
	m_CanAttack = false;
	m_CooldownTimer = m_CooldownDuration;
}


// Called every frame
void UASCloseCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!m_CanAttack)
	{
		CooldownTick(DeltaTime);
	}
}

void UASCloseCombatComponent::StartCloseCombatAttack()
{
	CooldownTrigger();
	const FVector Start = GetOwner()->GetActorLocation();
	const FVector ForwardVector = GetOwner()->GetActorForwardVector();
	const FVector End = Start + (ForwardVector * m_AttackRange);
    
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner()); // Ignore the attacking character in the sweep

	
	// Perform a sphere trace forward from the character to detect hits
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(m_DamageZoneRadius),
		CollisionParams
	);
	DrawDebugSphere(GetWorld(),End,m_DamageZoneRadius,12, FColor::Blue,false, 1, 0, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("Close Combat Attack Started"));
	if (bHit)
	{
		for (auto& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				UASHealthComponent* HealthComponent = UASHealthComponent::FindHealthComponent(HitActor);
				if(HealthComponent != nullptr)
				{
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_Sound_SuccessfulCloseCombatAttack, End);

					HealthComponent->Damage(m_Damage, GetOwner(), m_StunDuration);
					//UE_LOG(LogTemp, Warning, TEXT("CloseCombat hit %s"), *HitActor->GetName());
					//GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, TEXT("Target Hit"));
					return;
				}
			}
		}
	}
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_Sound_MissedCloseCombatAttack, End);
}

