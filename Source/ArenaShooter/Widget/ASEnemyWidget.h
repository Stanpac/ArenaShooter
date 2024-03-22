// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASEnemyWidget.generated.h"

class UProgressBar;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class ARENASHOOTER_API UASEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	UASEnemyWidget(const FObjectInitializer& ObjectInitializer);
	
};
