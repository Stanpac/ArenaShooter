// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Widget/ASGlobalWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"

UASGlobalWidget::UASGlobalWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Constructor
}

bool UASGlobalWidget::Initialize()
{
	SethealthBarPercent(1.0f);
	ChangeCursorVisibility(false);
	return Super::Initialize();
}

void UASGlobalWidget::UpdatehealthBar(float percent)
{
	// Manage the update of the health bar
	// Maybe Add a timer with Lerp ?
	SethealthBarPercent(percent);
}

void UASGlobalWidget::ChangeCursorVisibility(bool visible)
{
	// Manage the visibility of the cursor
	if (m_CursorImage) {
		m_CursorImage->SetVisibility(visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UASGlobalWidget::SethealthBarPercent(float percent)
{
	if (m_HealthBar) {
		m_HealthBar->SetPercent(percent);
	}
}
