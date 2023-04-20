// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PhaseHUD.h"
#include "HUD/PhaseOverlay.h"

void APhaseHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && PhaseOverlayClass)
		{
			PhaseOverlay = CreateWidget<UPhaseOverlay>(Controller, PhaseOverlayClass);
			PhaseOverlay->AddToViewport();
		}
	}
	
}
