// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTPROJECT_API IHitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* UFUNCTION(BlueprintNativeEvent) creates two GetHit functions, one that's callable in blueprints and one that's callable in C++. When calling the new GetHit function from C++, you must use this syntax: Execute_GetHit(). In each header/cpp file that uses GetHit() you must replace it with GetHit_Implementation(). This new function can be overridden even though it is not declared as virtural, this is because the new function created for GetHit() is hidden, but it is virtural.*/
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
};
