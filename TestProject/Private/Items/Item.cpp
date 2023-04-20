// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "TestProject/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	Sphere->SetVisibility(false);
	//UE_LOG(LogTemp, Warning, TEXT("Begin Play Called!"));

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("Item on screen message!"));
	}
	*/
	/*UWorld* World = GetWorld();
	FRotator RotateArrow = FRotator(0, 0, 0);
	FRotator RotateActor = FRotator(0.f, 45.f, 0.f);
	FVector MoveToLocation = FVector(0.f, 0.f, 300.f);
	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector EndLocation = Location + Forward * 200.f;
	
		
		SetActorLocation(MoveToLocation);
		SetActorRotation(RotateActor);
		Location = GetActorLocation();
		Forward = GetActorForwardVector();
		DRAW_SPHERE(Location);
		EndLocation = Location + Forward * 200.f;
		DRAW_ARROW_VECTOR(Location, EndLocation);
		
		DRAW_LINE(Location, Location + Forward * 100.f);
		DRAW_POINT(Location + Forward * 100.f);
		DRAW_VECTOR draws the same thing as the two commented lines of code above but simplified
		DRAW_VECTOR(Location, Location + Forward * 100.f);*/
	//int32 AvgInt = Avg<int32>(1, 3);
	//UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3: %d"), AvgInt);
	//float AvgFloat = Avg<float>(3.45f, 7.86f);
	//UE_LOG(LogTemp, Warning, TEXT("Avg of 3.45 and 7.86: %f"), AvgFloat);


}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation());
	}
}

void AItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, PickupSound, GetActorLocation());
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
	


	/*UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime)*/
	/*if (GEngine)
		{
			FString Name = GetName();
			FString Message = FString::Printf(TEXT("Item Name: %s"), *Name);
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, Message);
			UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Name);
		}*/
		/* Movement rate in units of cm/s
			float MovementRate = 50.f;
			float RotationRate = 45.f;

			MovementRate * DeltaTime (cm.s) * (s/frame) = (cm/frame)
			AddActorWorldOffset(FVector(MovementRate * DeltaTime, 0.f, 0.f));
			AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));*/
			/*float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);

				AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));*/
				/*DRAW_SPHERE_SingleFrame(GetActorLocation());
					DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);*/
					/*FVector AvgVector = Avg<FVector>(GetActorLocation(), FVector::ZeroVector);
						DRAW_POINT_SingleFrame(AvgVector);
						FRotator AvgRotator = Avg<FRotator>(GetActorRotation(), FRotator::ZeroRotator);*/

}