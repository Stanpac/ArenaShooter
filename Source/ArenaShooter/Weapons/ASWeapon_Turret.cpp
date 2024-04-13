#include "ASWeapon_Turret.h"

#include "ArenaShooter/AI/Drone/ASDronePawn.h"
#include "ArenaShooter/Character/ASCharacter.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnitConversion.h"


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
	
	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5);
	ETraceTypeQuery TraceTypeQuery2 = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
	GetWorld(),
	fireOrigin,
	fireOrigin + fireDirection * 1000000,
	.1f,
	TraceTypeQuery,
	false,
	ActorsToIgnore,
	EDrawDebugTrace::None,
	OutHit,
	true,
	FColor::Black
	);

	TArray<FHitResult> OutHits;
	
	bool bHit2 = UKismetSystemLibrary::SphereTraceMulti(
	GetWorld(),
	fireOrigin,
	OutHit.Location + (OutHit.Location - fireOrigin).GetSafeNormal() * 100,
	m_LaserRadius,
	TraceTypeQuery2,
	false,
	ActorsToIgnore,
	EDrawDebugTrace::None,
	OutHits,
	true,
	FColor::Black
	);
	
	if(bHit)
	{
		OnFireEvent.Broadcast(OutHit.Location);
	}

	if(bHit2)
	{
		for (auto hit : OutHits)
		{
			if(auto pawn = Cast<AASDronePawn>(hit.GetActor()))
			{
				UASHealthComponent::FindHealthComponent(pawn)->Damage(100, this);
			}
			
			if(auto pawn = Cast<ACharacter>(hit.GetActor()))
			{
				UASHealthComponent::FindHealthComponent(pawn)->Damage(GetWorld()->DeltaTimeSeconds * 200, this);
			}
		}
	}
}
