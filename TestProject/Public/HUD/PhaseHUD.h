// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PhaseHUD.generated.h"

class UPhaseOverlay;
/**
 * 
 */
UCLASS()
class TESTPROJECT_API APhaseHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = Phase)
	TSubclassOf<UPhaseOverlay> PhaseOverlayClass;
	
	UPROPERTY()
	UPhaseOverlay* PhaseOverlay;

public:
	FORCEINLINE UPhaseOverlay* GetPhaseOverlay() const { return PhaseOverlay; }
};
