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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASEnemyWidget", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> m_BaseCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASEnemyWidget", meta = (BindWidget))
	TObjectPtr<UProgressBar> m_HealthBar;

	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	UASEnemyWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	void UpdatehealthBar(float percent);

	void SetHealthBarColor(bool isVulnerable);
	
protected:
	void SethealthBarPercent(float percent);
};
