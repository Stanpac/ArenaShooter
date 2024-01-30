// Fill out your copyright notice in the Description page of Project Settings.


#include "ASWeapon.h"

#include "ArenaShooter/Components/ASHealthComponent.h"

// Sets default values
AASWeapon::AASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(m_IsReloading) ReloadTick(DeltaTime);
	if(m_IsWaitingForFireDelay) FireDelayTick(DeltaTime);
}

void AASWeapon::ReloadTick(float DeltaTime)
{
	m_ReloadTimer -= DeltaTime;
	if(m_ReloadTimer <= 0)
	{
		m_IsReloading = false;
		m_CurrentAmmos = m_MagazineSize;
	}
}

void AASWeapon::FireDelayTick(float DeltaTime)
{
	m_FireDelayTimer -= DeltaTime;
	if(m_FireDelayTimer <= 0)
	{
		m_IsWaitingForFireDelay = false;
	}
}

void AASWeapon::Equip()
{
}

void AASWeapon::Stach()
{
}

void AASWeapon::Fire(FVector fireOrigin, FVector fireDirection)
{
	FireDelayProc();
	
	if(m_EnableReload)
	{
		m_CurrentAmmos--;
		if(m_CurrentAmmos == 0) Reload();
	}
}

void AASWeapon::Reload()
{
	m_IsReloading = true;
	m_ReloadTimer = m_ReloadDuration;
}

void AASWeapon::FireDelayProc()
{
	m_IsWaitingForFireDelay = true;
	m_FireDelayTimer = m_FireDelayDuration;
}