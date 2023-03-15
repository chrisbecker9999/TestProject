// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
	/* Set this actor to call Tick() every frame.  You can turn this off to improve 
	performance if you don't need it.*/
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent> (TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	//Add actor when actor breaks
	//UFUNCTION()
	//GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &)
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken) return;
	bBroken = true;
	UWorld* World = GetWorld();

	if(World && TreasureClasses.Num() > 0)
	{
		FVector Location = GetActorLocation();
		FVector OriginalLocation = Location;
		FRotator ActorRotation = GetActorRotation();
		int32 RandomNumber = 0;
		int32 TreasureNum = 0;
		FString TreasureName = "";

		//Spawn rare extra pot if random number chosen is 1
		RandomNumber = UKismetMathLibrary::RandomIntegerInRange(1, 10);
		TreasureNum = UKismetMathLibrary::RandomIntegerInRange(0, TreasureClasses.Num() - 1);
		//Get the name of the selected TreasureClass
		TreasureClasses[TreasureNum]->GetDefaultObject()->AppendName(TreasureName);
		//Print a debug line showing the name of selected TreasureClass
		UE_LOG(LogTemp, Warning, TEXT("Treausre Name:  %s"), *TreasureName);
		
		/*Determine if the TreasureClass chosen was GoldPile, 
		if so do not change Z location, If not change the Z location.
		This is because the gold pile mesh does not have a bottom and
		must lay on the ground*/
		if (TreasureName == "Default__BP_GoldPile_C")
		{
			Location.Z += 0.f;
		}
		else
		{
			Location.Z += 75.f;
		}
		
		//Spawn the blueprint treasure class that was randomly selected
		World->SpawnActor<ATreasure>(TreasureClasses[TreasureNum], Location, ActorRotation);
		UE_LOG(LogTemp, Warning, TEXT("Treausre#:  %d"), TreasureNum);

		/*Sometimes spawn a additional blueprint treasure class randomly, 
		and also making sure that gold pile stays on the ground*/
		if (RandomNumber == 1)
		{
			if (TreasureName == "Default__BP_GoldPile_C")
			{
				Location.Z = OriginalLocation.Z;
			}
			else if (Location.Z != OriginalLocation.Z + 75.f)
			{
				Location.Z += 75.f;
			}

			//UE_LOG(LogTemp, Warning, TEXT("Vec: %s"), *Location.ToString());

			/*Offset the spawn vector of the extra treasure,
			that way the two treasures are not stacked ontop of each other*/
			Location.X += 100.f;
			Location.Y += 100.f;
			//Randomize the second treasure spawn
			TreasureNum = UKismetMathLibrary::RandomIntegerInRange(0, TreasureClasses.Num() - 1);
			//Spawn the extra treasure
			World->SpawnActor<ATreasure>(TreasureClasses[TreasureNum], Location, ActorRotation);
			
		}
		
	}
	
}

