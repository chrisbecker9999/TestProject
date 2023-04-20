// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PhaseOverlay.h"
#include "HUD/PhaseOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPhaseOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UPhaseOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UPhaseOverlay::SetGold(int32 Gold)
{
	if (GoldText)
	{	
		const FString String = FString::Printf(TEXT("%d"), Gold);
		const FText Text = FText::FromString(String);
		GoldText->SetText(Text);
	}
}

void UPhaseOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		const FString String = FString::Printf(TEXT("%d"), Souls);
		const FText Text = FText::FromString(String);
		SoulsText->SetText(Text);
	}
}
