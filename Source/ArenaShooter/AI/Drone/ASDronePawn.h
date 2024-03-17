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
	/*---------------------------------- MEMBERS --------------------------------------*/
protected:
	/** Player Reference */
	UPROPERTY()
	ACharacter* m_Player;

	/*---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASDronePawn();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void LookAtPlayer(float DeltaTime);
	
public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
