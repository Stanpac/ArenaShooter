// Fill out your copyright notice in the Description page of Project Settings.


#include "ASIASpawner.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


AASIASpawner::AASIASpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = m_RootComponent;

	m_SpawnZoneComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnZone"));
	if (m_SpawnZoneComponent) {
		m_SpawnZoneComponent->SetupAttachment(RootComponent);
		m_SpawnZoneComponent->SetSimulatePhysics(false);
		m_SpawnZoneComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_SpawnZoneComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->ArrowColor = FColor::Red;
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetSimulatePhysics(false);
	}
#endif // WITH_EDITORONLY_DATA
}

void AASIASpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ChangeSpawnZone();
}

void AASIASpawner::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);
}

void AASIASpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty"));
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AASIASpawner, m_SphereRadius) || PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AASIASpawner, m_BoxExtend)){
		UpdateSpawnZoneValues();
	}
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AASIASpawner, m_SpawnZone)) {
		ChangeSpawnZone();
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void AASIASpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (m_SpawnedAIAliveCount >= m_SpawnedAIAliveMax) {
		return;
	}
	
	if (m_SpawnType == ESpawnType::VE_SpawnOnGround) {
		SpawnAIOnGround();
	} else if (m_SpawnType == ESpawnType::VE_SpawnInTheAir) {
		SpawnAIInTheAir();
	}
}

void AASIASpawner::ChangeSpawnZone()
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeSpawnZone"));
	if (m_SpawnZone == ESPawnZone::VE_Box) {
		if (IsValid(m_SpawnZoneComponent) && !m_SpawnZoneComponent->IsA<UBoxComponent>()) {
			m_SpawnZoneComponent->DestroyComponent();
			m_SpawnZoneComponent = NewObject<UBoxComponent>(this);
			CastChecked<UBoxComponent>(m_SpawnZoneComponent)->SetBoxExtent(m_BoxExtend);
		} 
	} else if (m_SpawnZone == ESPawnZone::VE_Sphere) {
		if (IsValid(m_SpawnZoneComponent) && !m_SpawnZoneComponent->IsA<USphereComponent>()) {
			m_SpawnZoneComponent->DestroyComponent();
			m_SpawnZoneComponent = NewObject<USphereComponent>(this);
			CastChecked<USphereComponent>(m_SpawnZoneComponent)->SetSphereRadius(m_SphereRadius);
		} 
	}
}

void AASIASpawner::UpdateSpawnZoneValues()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateSpawnZoneValues"));
	if (m_SpawnZone == ESPawnZone::VE_Box) {
		if (IsValid(m_SpawnZoneComponent) && m_SpawnZoneComponent->IsA<UBoxComponent>()) {
			CastChecked<UBoxComponent>(m_SpawnZoneComponent)->SetBoxExtent(m_BoxExtend);
		}
	} else if (m_SpawnZone == ESPawnZone::VE_Sphere) {
		if (IsValid(m_SpawnZoneComponent) && m_SpawnZoneComponent->IsA<USphereComponent>()) {
			CastChecked<USphereComponent>(m_SpawnZoneComponent)->SetSphereRadius(m_SphereRadius);
		}
	}
}

void AASIASpawner::SpawnAI(FVector Location, FRotator Rotation)
{
	if (m_AIPawnClassTOSpawn.IsNull()) {
		UE_LOG(LogTemp, Warning, TEXT("AIPawnClassTOSpawn is NULL!"));
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APawn* pawn = GetWorld()->SpawnActor<APawn>(m_AIPawnClassTOSpawn.Get(), Location, Rotation, SpawnParams);
	if (pawn) {
		m_SpawnedAIAliveCount++;
		m_SpawnedAI.Add(pawn);
		pawn->OnDestroyed.AddDynamic(this, &AASIASpawner::OnAIDestroyed);
	}
}

void AASIASpawner::SpawnAIOnGround()
{
	// Find a random location in the zone of the m_SpawnZoneComponent and Start a Raycast, if the Raycast hit the ground, spawn the AI here and rotate it to the normal of the hit
	FVector Location = GetActorLocation();
	if (m_SpawnZoneComponent->IsA<UBoxComponent>()) {
		FVector BoxExtent = CastChecked<UBoxComponent>(m_SpawnZoneComponent)->GetScaledBoxExtent();
		Location = GetActorLocation() + FVector(FMath::RandRange(-BoxExtent.X, BoxExtent.X), FMath::RandRange(-BoxExtent.Y, BoxExtent.Y), 0);
	} else if (m_SpawnZoneComponent->IsA<USphereComponent>()) {
		// Get a random point in the sphere
		FVector RandomPoint = FMath::VRand() * m_SphereRadius;
		Location = GetActorLocation() + RandomPoint;
	}

	FHitResult Hit;
	FVector Start = Location;
	FVector End = Location - GetActorUpVector() * 1000;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams)) {
		SpawnAI(Hit.Location, Hit.ImpactNormal.Rotation());
	}
}

void AASIASpawner::SpawnAIInTheAir()
{
	// Find a random location in the zone of the m_SpawnZoneComponent and spawn the AI if
	FVector Location = GetActorLocation();
	if (m_SpawnZoneComponent->IsA<UBoxComponent>()) {
		FVector BoxExtent = CastChecked<UBoxComponent>(m_SpawnZoneComponent)->GetScaledBoxExtent();
		Location = GetActorLocation() + FVector(FMath::RandRange(-BoxExtent.X, BoxExtent.X), FMath::RandRange(-BoxExtent.Y, BoxExtent.Y), 0);
	} else if (m_SpawnZoneComponent->IsA<USphereComponent>()) {
		// Get a random point in the sphere
		FVector RandomPoint = FMath::VRand() * m_SphereRadius;
		Location = GetActorLocation() + RandomPoint;
	}
	
	SpawnAI(Location, GetActorRotation());
}

void AASIASpawner::OnAIDestroyed(AActor* DestroyedActor)
{
	if (APawn* pawn = Cast<APawn>(DestroyedActor)) {
		m_SpawnedAIAliveCount--;
		m_SpawnedAI.Remove(pawn);
	}
}






