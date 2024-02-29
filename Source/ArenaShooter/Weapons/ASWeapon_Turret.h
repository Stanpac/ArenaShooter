// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASWeapon.h"
#include "ASWeapon_Turret.generated.h"


class AASTurretBullet;

UCLASS()
class ARENASHOOTER_API AASWeapon_Turret : public AASWeapon
{
	GENERATED_BODY()
	virtual void Fire(FVector FireOrigin, FVector FireDirection) override;

public:
	/** Blueprint reference to the projectile Spawned */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay")
	TSubclassOf<AASTurretBullet> m_BulletBP;
};
