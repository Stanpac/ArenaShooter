#include "ASWeapon_Turret.h"

#include "ArenaShooter/AI/Drone/ASDronePawn.h"
#include "ArenaShooter/Character/ASCharacter.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void AASWeapon_Turret::BeginPlay()
{
	Super::BeginPlay();
	m_Character = Cast<AASCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
}

void AASWeapon_Turret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	m_Deltatime = DeltaSeconds;
}

void AASWeapon_Turret::Fire(FVector fireOrigin, FVector fireDirection)
{
	Super::Fire(fireOrigin, fireDirection);


	/*FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bIgnoreTouches = false;
	CollisionParams.AddIgnoredActor(GetOwner());

	if (bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,fireOrigin,fireOrigin + fireDirection * 1000000,ECC_Visibility, CollisionParams))
	{
		//if(OnFireEvent.IsBound())
			OnFireEvent.Broadcast(HitResult.Location);
	}*/


	FHitResult OutHit;
	TArray<AActor*> ActorsToIgnore {GetOwner()};
	
	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3);
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
	GetWorld(),
	fireOrigin,
	fireOrigin + fireDirection * 1000000,
	m_LaserRadius,
	TraceTypeQuery,
	false,
	ActorsToIgnore,
	EDrawDebugTrace::ForOneFrame,
	OutHit,
	true,
	FColor::Black
	);

	if(bHit)
	{
		OnFireEvent.Broadcast(OutHit.Location);
		if(auto pawn = Cast<AASDronePawn>(OutHit.GetActor()))
		{
			UASHealthComponent::FindHealthComponent(pawn)->Damage(100, this);
		}
		else if(OutHit.GetActor() == m_Character)
		{
			UASHealthComponent::FindHealthComponent(m_Character)->Damage(m_Deltatime * m_DamageByBullet, this);
		}
	}
}
