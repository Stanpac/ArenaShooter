// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/AI/ASPawn.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "ArenaShooter/SubSystem/ASEventWorldSubSystem.h"
#include "ArenaShooter/Widget/ASEnemyWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"



AASPawn::AASPawn()
{
	m_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	m_CapsuleComponent->SetCollisionProfileName("Pawn");

	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	m_MeshComponent->SetupAttachment(m_CapsuleComponent);

	m_HealthComponent = CreateDefaultSubobject<UASHealthComponent>(TEXT("HealthComponent"));
	m_WeaponComponent = CreateDefaultSubobject<UASWeaponComponent>(TEXT("WeaponComponent"));
}

void AASPawn::Stun(float stunDuration)
{
	m_IsStunned = true;
	m_StunTimer = stunDuration;
}

void AASPawn::BeginPlay()
{
	Super::BeginPlay();
	
	m_EventWorldSubSystem = GetWorld()->GetSubsystem<UASEventWorldSubSystem>();
	
	m_HealthComponent->OnHealthChanged.AddDynamic(this, &AASPawn::OnHealthChanged);
	m_HealthComponent->OnDeathStarted.AddDynamic(this, &AASPawn::OnDeath);

	m_WeaponComponent->InitializeWeapon();
	m_WeaponComponent->m_ASPawnOwner = this;
}

void AASPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(m_IsStunned) StunTick(DeltaSeconds);
}

void AASPawn::OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer)
{
	// Find a Way to get the Indicate that you can one shoot the enemy
	// Oeverlay Mat ?
	
	SpawnFloatingDamage(GetActorLocation(), GetActorRotation(), PreviousHealth - CurrentHealth);
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_ImpactParticle, GetActorLocation());
}

void AASPawn::OnDeath(AActor* DeathDealer)
{
	if (m_EventWorldSubSystem) {
		m_EventWorldSubSystem->BroadcastEnemyDeath();
	}
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_DeathParticle, GetActorLocation());
	
	SetLifeSpan(0.1f);
	SetActorHiddenInGame(true);
}

void AASPawn::StunTick(float DeltaTime)
{
	m_StunTimer -= DeltaTime;
	if(m_StunTimer <= 0)
	{
		m_IsStunned = false;
	}
}

void AASPawn::SpawnFloatingDamage(const FVector& SpawnLocation, const FRotator& SpawnRotation, const float Damage)
{
	if (GetWorld() == nullptr) {
		UE_LOG(LogTemp, Error,TEXT("%s, can't spawn floating damage. No context"), *GetNameSafe(this));
		return;
	}

	if (m_floatingDamageSystem == nullptr) {
		UE_LOG(LogTemp, Error,TEXT("%s, can't spawn floating damage. The system is null"), *GetNameSafe(this));
		return;
	}

	UNiagaraComponent* lNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_floatingDamageSystem, SpawnLocation, SpawnRotation, FVector::One(), true, true);
	
	FVector4 lDamageData = FVector4();

	lDamageData.X = SpawnLocation.X;
	lDamageData.Y = SpawnLocation.Y;
	lDamageData.Z = SpawnLocation.Z;
	lDamageData.W = Damage;
	
	/*Set the damage value into the system, it will use to compute where the texture number should take float*/
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4Value(lNiagaraComp, FName("DamageInfo"), 0, lDamageData, false);
}


