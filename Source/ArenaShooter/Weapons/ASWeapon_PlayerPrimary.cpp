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
	if(m_Sound_ShotFired) {
			UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			m_Sound_ShotFired,
			GetOwner()->GetActorLocation());
	}

	if (bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,FireOrigin,FireOrigin + FireDirection * 1000000,ECC_Visibility, CollisionParams)) {
		// Process the hit result
		if (AActor* HitActor = HitResult.GetActor()) {
			if(UASHealthComponent* HealthComponent = UASHealthComponent::FindHealthComponent(HitActor)) {
				HealthComponent->Damage(m_DamageByBullet, GetOwner(), m_OnHitStunDuration);
			} else {
				UGameplayStatics::SpawnDecalAtLocation(GetWorld(), m_DecalMaterial, m_DecalSize, FVector(HitResult.ImpactPoint), HitResult.ImpactNormal.Rotation().GetInverse(), 5.0f);
			}
		} 
	} else {
		// nothing is Hit
	}
}
