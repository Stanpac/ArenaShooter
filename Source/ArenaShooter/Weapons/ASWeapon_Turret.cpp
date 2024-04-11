#include "ASWeapon_Turret.h"



void AASWeapon_Turret::Fire(FVector fireOrigin, FVector fireDirection)
{
	Super::Fire(fireOrigin, fireDirection);


	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bIgnoreTouches = false;
	CollisionParams.AddIgnoredActor(GetOwner());

	if (bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,fireOrigin,fireOrigin + fireDirection * 1000000,ECC_Visibility, CollisionParams))
	{
		//if(OnFireEvent.IsBound())
			OnFireEvent.Broadcast(HitResult.Location);
	}
}
