// Fill out your copyright notice in the Description page of Project Settings.


#include "ASTurretBullet.h"

#include "ArenaShooter/Components/ASHealthComponent.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AASTurretBullet::AASTurretBullet(): m_Speed(10)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_HealthComponent = CreateDefaultSubobject<UASHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void AASTurretBullet::BeginPlay()
{
	Super::BeginPlay();
	m_Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	const FVector LookAtLocation = m_Character->GetActorLocation();
	const FVector PawnLocation = GetActorLocation();
	const FRotator LookAtRotation = (LookAtLocation - PawnLocation).Rotation();
	SetActorRotation(LookAtRotation);
	m_HealthComponent->OnDeathStarted.AddDynamic(this, &AASTurretBullet::OnDeath);
}

// Called every frame
void AASTurretBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovementForward(DeltaTime);
}

void AASTurretBullet::MovementToCharacter(float DeltaTime)
{
	const FVector startPosition = GetActorLocation();
	const FVector endPosition = m_Character->GetActorLocation();
	const float distance = FVector::Distance(startPosition, endPosition);
	const float distanceToTravel = m_Speed / distance * DeltaTime;
	const FVector newPosition = startPosition + (endPosition - startPosition) * distanceToTravel;

	SetActorLocation(newPosition, true); 
	
}

void AASTurretBullet::MovementForward(float DeltaTime)
{
	const FVector startPosition = GetActorLocation();
	const FVector endPosition = startPosition + GetActorForwardVector() * 100;
	const FVector newPosition = FMath::Lerp(startPosition, endPosition, DeltaTime * m_bulletSpeed);
	SetActorLocation(newPosition);
	
}

void AASTurretBullet::OnDeath(AActor* actor)
{
	m_HealthComponent->Death();
}

