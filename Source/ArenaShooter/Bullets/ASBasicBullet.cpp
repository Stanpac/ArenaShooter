// Fill out your copyright notice in the Description page of Project Settings.


#include "ASBasicBullet.h"

// Sets default values
AASBasicBullet::AASBasicBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASBasicBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASBasicBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		const FVector startPosition = GetActorLocation();
		const FVector endPosition = startPosition + GetActorForwardVector() * 100;
		const FVector newPosition = FMath::Lerp(startPosition, endPosition, DeltaTime * m_bulletSpeed);
		SetActorLocation(newPosition, true);
}

