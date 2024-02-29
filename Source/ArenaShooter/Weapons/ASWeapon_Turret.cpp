// Fill out your copyright notice in the Description page of Project Settings.


#include "ASWeapon_Turret.h"

#include "ArenaShooter/Bullets/ASTurretBullet.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AASWeapon_Turret::Fire(FVector FireOrigin, FVector FireDirection)
{
	Super::Fire(FireOrigin, FireDirection);
	FActorSpawnParameters params;
	params.ObjectFlags |= RF_Transient;
	params.bNoFail = true;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if(IsValid(m_BulletBP))
	{
		const ACharacter* m_Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		const FVector LookAtLocation = m_Character->GetActorLocation();
		const FRotator LookAtRotation = (LookAtLocation - FireOrigin).Rotation();
		AASTurretBullet* bullet = GetWorld()->SpawnActor<AASTurretBullet>(m_BulletBP, FireOrigin ,LookAtRotation, params);
		bullet->m_bulletSpeed = m_bulletSpeed;
	}
}
