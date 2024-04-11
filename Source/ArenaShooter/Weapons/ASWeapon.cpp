// Fill out your copyright notice in the Description page of Project Settings.


#include "ASWeapon.h"

#include "ArenaShooter/Components/ASHealthComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AASWeapon::AASWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_FireMuzzleComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");

	M_WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	M_WeaponMesh->SetupAttachment(m_FireMuzzleComponent);


	
	m_FireShootPos = CreateDefaultSubobject<USceneComponent>("FireShootPosition");
	m_FireShootPos->SetupAttachment(M_WeaponMesh, "FireShootPosition");
	
	m_Niagara_ShotFiredComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	m_Niagara_ShotFiredComponent->SetupAttachment(M_WeaponMesh, "FireShootPosition");
}

// Called when the game starts or when spawned
void AASWeapon::BeginPlay()
{
	Super::BeginPlay();
	if(!m_ShowWeapon) SetHidden(true);
	//FRotator CurrentRotation = M_WeaponMesh->GetComponentRotation();
	//CurrentRotation.Roll += m_Offset.X;
	//CurrentRotation.Pitch += m_Offset.Y;
	//CurrentRotation.Yaw += m_Offset.Z;		
	//M_WeaponMesh->SetWorldRotation(CurrentRotation);
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

void AASWeapon::Fire(FVector fireOrigin, FVector fireDirection)
{
	FireDelayProc();

	if (m_Niagara_ShotFiredComponent){
		m_Niagara_ShotFiredComponent->DeactivateImmediate();
		m_Niagara_ShotFiredComponent->Activate();
	}
	
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
