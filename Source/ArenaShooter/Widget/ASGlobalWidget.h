// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASGlobalWidget.generated.h"

class UImage;
class UCanvasPanel;
class UProgressBar;

/**
 *  Class for the Widget that will be displayed on the screen
 *  All The gameplay UI will be in this widget
 */
UCLASS()
class ARENASHOOTER_API UASGlobalWidget : public UUserWidget
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASGlobalWidget", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> m_BaseCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASGlobalWidget", meta = (BindWidget))
	TObjectPtr<UProgressBar> m_HealthBar;

	UPROPERTY(editanywhere, BlueprintReadWrite, Category = "ASGlobalWidget", meta = (BindWidget))
	TObjectPtr<UImage> m_CursorImage;
	
	/* ---------------------------------- FUNCTIOns --------------------------------------*/
public:
	UASGlobalWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	void UpdatehealthBar(float percent);

	void ChangeCursorVisibility(bool visible);

protected:
	void SethealthBarPercent(float percent);
};

