// Fill out your copyright notice in the Description page of Project Settings.


#include "ASIASpawner.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


AASIASpawner::AASIASpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
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
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AASIASpawner, m_SpawnZone)) {
		ChangeSpawnZone();
	} 
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
	
	if (m_SpawnZone == ESPawnZone::VE_Box) {
		if (!m_SpawnZoneComponent->IsA<UBoxComponent>()) {
			if (m_SpawnZoneComponent) m_SpawnZoneComponent->DestroyComponent();
			m_SpawnZoneComponent = NewObject<UBoxComponent>(this);
			CastChecked<UBoxComponent>(m_SpawnZoneComponent)->SetBoxExtent(m_BoxExtend);
		}
	} else if (m_SpawnZone == ESPawnZone::VE_Sphere) {
		if (!m_SpawnZoneComponent->IsA<USphereComponent>()) {
			if (m_SpawnZoneComponent) m_SpawnZoneComponent->DestroyComponent();
			m_SpawnZoneComponent = NewObject<USphereComponent>(this);
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
	if (m_SpawnZoneComponent->IsA<UBoxComponent>()) {
		// Cast a raycast from the center of the box to the ground
	} else if (m_SpawnZoneComponent->IsA<USphereComponent>()) {
		// Cast a raycast from the center of the sphere to the ground
	}
}

void AASIASpawner::SpawnAIInTheAir()
{
	// Find a random location in the zone of the m_SpawnZoneComponent and spawn the AI if
	if (m_SpawnZoneComponent->IsA<UBoxComponent>()) {
		
	} else if (m_SpawnZoneComponent->IsA<USphereComponent>()) {
		
	}
}

void AASIASpawner::OnAIDestroyed(AActor* DestroyedActor)
{
	if (APawn* pawn = Cast<APawn>(DestroyedActor)) {
		m_SpawnedAIAliveCount--;
		m_SpawnedAI.Remove(pawn);
	}
}






