// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	/*Must have the same name that it has in the WBP_HealthBar blueprint 
	class we created in UE5. File: Blueprints/WBP_HealthBar 
	Hierarchy: WBP_HealthBar->Canvas Panel->HealthBar which is a progress bar widget */
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
};
