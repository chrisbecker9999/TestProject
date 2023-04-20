// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealthPotion.h"
#include "Interfaces/PickupInterface.h"
#include "Characters/ParagonPhase/ParagonPhase.h"

// Sets default values

void AHealthPotion::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->AddHealth(this);
		SpawnPickupSystem();
		SpawnPickupSound();
		Destroy();
	}
}
