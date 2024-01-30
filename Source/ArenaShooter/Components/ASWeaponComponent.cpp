// Fill out your copyright notice in the Description page of Project Settings.


#include "ASWeaponComponent.h"
#include "Components/WeaponComponent.h"

#include "ASHealthComponent.h"

UWeaponComponent::UASWeaponComponent()
{
}

void UWeaponComponent::Fire(FVector direction, FVector startingPos)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		startingPos,
		startingPos + direction * 10000,
		ECC_Visibility, // Change this collision channel as needed
		CollisionParams
	);

	if (bHit)
	{
		// Process the hit result
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{
			UASHealthComponent* HealthComponent = UASHealthComponent::FindHealthComponent(HitActor);
			if(HealthComponent != nullptr)
			{
				HealthComponent->Damage(10);
			}
		}
	}
}
