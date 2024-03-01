// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Widget/ASEnemyWidget.h"

#include "Components/ProgressBar.h"

UASEnemyWidget::UASEnemyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//Constructor
}

bool UASEnemyWidget::Initialize()
{
	SethealthBarPercent(1.0f);
	return Super::Initialize();
}

void UASEnemyWidget::UpdatehealthBar(float percent)
{
	SethealthBarPercent(percent);
}

void UASEnemyWidget::SetHealthBarColor(bool isVulnerable)
{
	if(isVulnerable)
		m_HealthBar->SetFillColorAndOpacity(FColor::Red);
	else
		m_HealthBar->SetFillColorAndOpacity(FColor::Green);
}

void UASEnemyWidget::SethealthBarPercent(float percent)
{
	if (m_HealthBar) {
		m_HealthBar->SetPercent(percent);
	}
}
