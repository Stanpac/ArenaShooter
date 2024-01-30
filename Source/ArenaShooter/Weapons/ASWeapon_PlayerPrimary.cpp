#include "ASWeapon_PlayerPrimary.h"
#include "ArenaShooter/Components/ASHealthComponent.h"

void AASWeapon_PlayerPrimary::Fire(FVector FireOrigin, FVector FireDirection)
{
	Super::Fire(FireOrigin, FireDirection);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, TEXT("Fire triggered"));
	
	if (bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		FireOrigin,
		FireOrigin + FireDirection * 1000000,
		ECC_PhysicsBody, // Change this collision channel as needed
		CollisionParams))
	{
		// Process the hit result
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Something is hit"));
		if (HitActor != nullptr) {
			DrawDebugLine(GetWorld(),FireOrigin,HitResult.ImpactPoint ,FColor::Red,false,1.0f,0,1.0f);
			
			UASHealthComponent* HealthComponent = UASHealthComponent::FindHealthComponent(HitActor);
			if(HealthComponent != nullptr) {
				HealthComponent->Damage(10);
				GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, TEXT("Target Hit"));
			}
		} else {
			DrawDebugLine(GetWorld(),FireOrigin,FireOrigin + FireDirection * 100000,FColor::Red,false,1.0f,0,1.0f);
		}
	} else {
		DrawDebugLine(GetWorld(),FireOrigin,FireOrigin + FireDirection * 100000,FColor::Red,false,1.0f,0,1.0f);
	}
}
