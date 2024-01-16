// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASCharacter.generated.h"

UCLASS()
class ARENASHOOTER_API AASCharacter : public ACharacter
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
protected:
	
public:
	AASCharacter();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
