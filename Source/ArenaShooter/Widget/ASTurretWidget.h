// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASTurretWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API UASTurretWidget : public UUserWidget
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	UASTurretWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent)
	void SetHealthBarPercent(float Health, float MaxHealth);
	
};
