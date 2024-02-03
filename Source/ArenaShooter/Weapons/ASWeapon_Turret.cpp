// Fill out your copyright notice in the Description page of Project Settings.


#include "ASWeapon_Turret.h"

#include "ArenaShooter/Bullets/ASTurretBullet.h"

void AASWeapon_Turret::Fire(FVector FireOrigin, FVector FireDirection)
{
	Super::Fire(FireOrigin, FireDirection);
	FActorSpawnParameters params;
	params.ObjectFlags |= RF_Transient;
	params.bNoFail = true;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if(IsValid(m_BulletBP))
	{
		AASTurretBullet* bullet =  GetWorld()->SpawnActor<AASTurretBullet>(m_BulletBP, params);
		bullet->SetActorLocation(FireOrigin);
	}
}
