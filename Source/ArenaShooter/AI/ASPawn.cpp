// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/AI/ASPawn.h"

#include "NiagaraFunctionLibrary.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "ArenaShooter/SubSystem/ASEventWorldSubSystem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"

AASPawn::AASPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(m_RootComponent);
	
	m_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	m_CapsuleComponent->SetCollisionProfileName("Pawn");
	m_CapsuleComponent->SetupAttachment(m_RootComponent);

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
	if (PreviousHealth == CurrentHealth) return;

	if(m_ImpactParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_ImpactParticle, m_HitLocation, m_HitRotation);
	}

	if(IsValid(m_SoundHit))
	{
		UGameplayStatics::PlaySoundAtLocation( GetWorld(), m_SoundHit, GetActorLocation());
	}
	
	SpawnFloatingDamage(GetActorLocation(), GetActorRotation(), PreviousHealth - CurrentHealth);
}

void AASPawn::OnDeath(AActor* DeathDealer)
{
	if (m_EventWorldSubSystem) {
		m_EventWorldSubSystem->BroadcastEnemyDeath();
	}

	if(IsValid(m_SoundDeath))
	{
		UGameplayStatics::PlaySoundAtLocation( GetWorld(), m_SoundDeath, GetActorLocation());
	}
	
	if(IsValid(m_DeathParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_DeathParticle, GetActorLocation());
	}
	
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

void AASPawn::ManageIndicatorWidget()
{
	if(m_IndicatorWidget == nullptr) return;
	
	FVector2D lScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), GetActorLocation(), lScreenPosition);
	if (!WasRecentlyRendered(0.1f)) {
		m_IndicatorWidget->SetVisibility(ESlateVisibility::Visible);
	} else {
		m_IndicatorWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	FVector2D lDesiredSize =FVector2D(GEngine->GameViewport->Viewport->GetSizeXY()) - m_IndicatorWidget->GetDesiredSize();
	lScreenPosition.X = FMath::Clamp(lScreenPosition.X, 0, lDesiredSize.X);
	lScreenPosition.Y = FMath::Clamp(lScreenPosition.Y, 0, lDesiredSize.Y);
	
	m_IndicatorWidget->SetPositionInViewport(lScreenPosition);
}

/*(const FVector& SpawnLocation, const FRotator& SpawnRotation, const float Damage)
{
	
	UNiagaraComponent* lNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_floatingDamageSystem, SpawnLocation, SpawnRotation, FVector::One(), true, true);
	FVector4 lDamageData = FVector4();
	lDamageData.X = SpawnLocation.X;
	lDamageData.Y = SpawnLocation.Y;
	lDamageData.Z = SpawnLocation.Z;
	lDamageData.W = Damage;
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4Value(lNiagaraComp, FName("DamageInfo"), 0, lDamageData, false);
}*/

void AASPawn::SetHitPosition(const FVector& hitPosition)
{
	m_HitLocation = hitPosition;
}

void AASPawn::SetHitRotation(const FRotator& hitRotation)
{
	m_HitRotation = hitRotation;
}


