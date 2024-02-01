// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/AI/ASPawn.h"

#include "ArenaShooter/Components/ASHealthComponent.h"
#include "Components/CapsuleComponent.h"


AASPawn::AASPawn()
{
	m_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	m_CapsuleComponent->SetCollisionProfileName("Pawn");
	RootComponent = m_CapsuleComponent;

	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	m_MeshComponent->SetupAttachment(m_CapsuleComponent);

	m_HealthComponent = CreateDefaultSubobject<UASHealthComponent>(TEXT("HealthComponent"));
}


