// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Charecters/ParagonPhase/ParagonPhase.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AParagonPhase* ParagonPhase = Cast<AParagonPhase>(OtherActor);
	if (ParagonPhase)
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}
		Destroy();
	}
}
