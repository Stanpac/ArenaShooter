// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/AI/ASPawn.h"

#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Widget/ASEnemyWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"


AASPawn::AASPawn()
{
	m_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	m_CapsuleComponent->SetCollisionProfileName("Pawn");
	RootComponent = m_CapsuleComponent;

	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	m_MeshComponent->SetupAttachment(m_CapsuleComponent);

	m_HealthComponent = CreateDefaultSubobject<UASHealthComponent>(TEXT("HealthComponent"));

	m_HealthVisibilitySphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HealthVisibilitySphereComponent"));
	m_HealthVisibilitySphereComponent->SetupAttachment(m_CapsuleComponent);
	m_HealthVisibilitySphereComponent->SetSphereRadius(1000.0f);
	
	m_HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	m_HealthBarWidgetComponent->SetupAttachment(m_CapsuleComponent);
	m_HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	m_HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
}

void AASPawn::BeginPlay()
{
	Super::BeginPlay();
	m_HealthBarWidgetComponent->SetWidgetClass(m_HealthBarWidgetClass);
	m_HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
	SetWidgetVisibility(false);
	
	m_HealthVisibilitySphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AASPawn::OnHealthVisibilitySphereComponentBeginOverlap);
	m_HealthVisibilitySphereComponent->OnComponentEndOverlap.AddDynamic(this, &AASPawn::OnHealthVisibilitySphereComponentEndOverlap);
	
	m_HealthComponent->OnHealthChanged.AddDynamic(this, &AASPawn::OnHealthChanged);
	m_HealthComponent->OnDeathStarted.AddDynamic(this, &AASPawn::OnDeath);
}

void AASPawn::OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer)
{
	if (m_HealthBarWidgetComponent) {
		
		if (DamageDealer != this && !m_HealthBarWidgetComponent->IsWidgetVisible()) {
			SetWidgetVisibility(true);
			GetWorldTimerManager().SetTimer(m_HealthBarTimerHandle, this, &AASPawn::SetWidgetVisibilityfalse, 1, false);
			// Launch Timer For the HealthBar
		}
		
		if (UASEnemyWidget* GlobalWidget = CastChecked<UASEnemyWidget>(m_HealthBarWidgetComponent->GetUserWidgetObject())) {
			GlobalWidget->UpdatehealthBar(CurrentHealth / MaxHealth);
		}
	}
	
	// TODO Spawn Decal of the Damage
	
}

void AASPawn::OnDeath(AActor* DeathDealer)
{
	SetLifeSpan(0.1f);
}

void AASPawn::SetWidgetVisibility(bool visible)
{
	if (m_HealthBarWidgetComponent) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Setvisibility");
		m_HealthBarWidgetComponent->SetVisibility(visible);
	}
}

void AASPawn::SetWidgetVisibilityfalse()
{
	if (m_HealthBarWidgetComponent) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Setvisibilityfalse");
		m_HealthBarWidgetComponent->SetVisibility(false);
	}
}

void AASPawn::OnHealthVisibilitySphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "OnHealthVisibilitySphereComponentBeginOverlap");
	if (OtherActor == this) {
		return;
	}
	SetWidgetVisibility(true);
}

void AASPawn::OnHealthVisibilitySphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "OnHealthVisibilitySphereComponentEndOverlap");
	if (OtherActor == this) {
		return;
	}
	SetWidgetVisibility(false);
}


