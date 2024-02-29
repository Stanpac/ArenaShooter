// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaShooter/AI/ASPawn.h"
#include "GameFramework/Pawn.h"
#include "ASDronePawn.generated.h"

class UASHealthComponent;
class UASWeaponComponent;

UCLASS()
class ARENASHOOTER_API AASDronePawn : public AASPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AASDronePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Player Reference */
	UPROPERTY()
	ACharacter* m_Player;

	UFUNCTION()
	virtual void LookAtPlayer(float DeltaTime);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
