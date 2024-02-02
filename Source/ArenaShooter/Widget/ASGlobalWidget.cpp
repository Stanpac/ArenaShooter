// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/Widget/ASGlobalWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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

void UASGlobalWidget::SetSpeedBarPercent(float percent)
{
	if (m_SpeedBar) {
		m_SpeedBar->SetPercent(percent);
	}
}

void UASGlobalWidget::SethealthBarPercent(float percent)
{
	if (m_HealthBar) {
		m_HealthBar->SetPercent(percent);
	}
}

void UASGlobalWidget::ChangeCursorVisibility(bool visible)
{
	// Manage the visibility of the cursor
	if (m_CursorImage) {
		m_CursorImage->SetVisibility(visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UASGlobalWidget::SetSpeedProfile(int profile)
{
	if (m_SpeedProfile) {
		m_SpeedProfile->SetText(FText::FromString(FString::FromInt(profile)));
	}
}

