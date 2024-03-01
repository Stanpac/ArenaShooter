#include "ASWeapon_PlayerPrimary.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "Kismet/GameplayStatics.h"

void AASWeapon_PlayerPrimary::Fire(FVector FireOrigin, FVector FireDirection)
{
	Super::Fire(FireOrigin, FireDirection);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bIgnoreTouches = false;
	CollisionParams.AddIgnoredActor(GetOwner());
	if(m_Sound_ShotFired)
		{
			UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			m_Sound_ShotFired,
			GetOwner()->GetActorLocation());
		}
	if (bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		FireOrigin,
		FireOrigin + FireDirection * 1000000,
		ECC_Visibility, 
		CollisionParams))
	{
		// Process the hit result
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr) {
			DrawDebugLine(GetWorld(),FireOrigin,HitResult.ImpactPoint ,FColor::Red,false,1.0f,0,1.0f);
			
			UASHealthComponent* HealthComponent = UASHealthComponent::FindHealthComponent(HitActor);
			if(HealthComponent != nullptr) {
				
				HealthComponent->Damage(m_DamageByBullet, GetOwner(), m_OnHitStunDuration);
			}
		} else {
			DrawDebugLine(GetWorld(),FireOrigin,FireOrigin + FireDirection * 100000,FColor::Red,false,1.0f,0,1.0f);
		}
	} else {
		DrawDebugLine(GetWorld(),FireOrigin,FireOrigin + FireDirection * 100000,FColor::Red,false,1.0f,0,1.0f);
	}
}
