// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ParagonPhase/PhaseAnimInstance.h"
#include "Characters/ParagonPhase/ParagonPhase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UPhaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	PhaseCharacter = Cast<AParagonPhase>(TryGetPawnOwner());
	if (PhaseCharacter)
	{
		PhaseCharacterMovement = PhaseCharacter->GetCharacterMovement();
	}
}

void UPhaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PhaseCharacterMovement)
	{
		
		GroundSpeed = UKismetMathLibrary::VSizeXY(PhaseCharacterMovement->Velocity);
		IsFalling = PhaseCharacterMovement->IsFalling();
		CharacterState = PhaseCharacter->GetCharacterState();
	}
}
