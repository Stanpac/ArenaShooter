// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASGlobalWidget.generated.h"

/**
 *  Class for the Widget that will be displayed on the screen
 *  All The gameplay UI will be in this widget
 */
UCLASS()
class ARENASHOOTER_API UASGlobalWidget : public UUserWidget
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	UASGlobalWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintImplementableEvent)
	void SethealthBarPercent(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void ManageCursor(AActor* Target);

	UFUNCTION(BlueprintImplementableEvent)
	void SetGravityAbilityWidget(bool visible);

	UFUNCTION(BlueprintImplementableEvent)
	void SetGravityChargeBarPercent(float percent);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetNbrOfCharge(int NbrOfCharge);

	UFUNCTION(BlueprintImplementableEvent)
	void SetDashAbilityWidget(bool visible);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetDashChargeBarPercent(float percent);
};

