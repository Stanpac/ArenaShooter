// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Character/ASCharacter.h"


AASCharacter::AASCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

