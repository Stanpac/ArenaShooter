// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Widget/ASGlobalWidget.h"

#include "Components/ProgressBar.h"

UASGlobalWidget::UASGlobalWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Constructor
}

bool UASGlobalWidget::Initialize()
{
	SethealthBarPercent(1.0f);
	return Super::Initialize();
}

void UASGlobalWidget::UpdatehealthBar(float percent)
{
	// Manage the update of the health bar
	// Maybe Add a timer with Lerp ?
	SethealthBarPercent(percent);
}

void UASGlobalWidget::SethealthBarPercent(float percent)
{
	if (m_HealthBar) {
		m_HealthBar->SetPercent(percent);
	}
}
