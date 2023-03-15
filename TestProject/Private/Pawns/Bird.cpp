// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ABird::ABird()
{
	//UE_LOG(LogTemp, Warning, TEXT("I made it inside ABird::ABird()!"));
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("I made it inside BeginPlay() PlayerController IF statement!"));
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("I made it inside BeginPlay() SubSystem IF statement!"));
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
}

//void ABird::Turn(float Value)
//{
//}
//
//void ABird::LookUp(float Value)
//{
//}

//void ABird::MoveForward(float Value)
//{
//	//UE_LOG(LogTemp, Warning, TEXT("I made it inside ABird::MoveForward!"))
//
//	if (Controller && (Value != 0.f))
//	{
//		//UE_LOG(LogTemp, Warning, TEXT("I made it inside ABird::MoveForward IF statement!"));
//		FVector Forward = GetActorForwardVector();
//		AddMovementInput(Forward, Value);
//	}
//}

void ABird::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if(GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

//void ABird::SpeedBoost(const UFloatingPawnMovement& Value)
//{
//	const float MaxSpeedValue = Value.Get<float>();
//
//	if (Controller)
//	{
//		
//	}
//
//}

				
void ABird::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();

	//UE_LOG(LogTemp, Warning, TEXT("I made it inside ABird::Move!"));

	if (Controller && (DirectionValue != 0.f))
	{
		//UE_LOG(LogTemp, Warning, TEXT("I made it inside ABird::Move IF statement!"));
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, DirectionValue);
		//UE_LOG(LogTemp, Warning, TEXT("%f"), DirectionValue);
	}
}



// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//UE_LOG(LogTemp, Warning, TEXT("I made it inside ABird::SetupPlayerInputComponent!"));
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//UE_LOG(LogTemp, Warning, TEXT("I made it inside ABird::SetupPlayerInputComponent IF statement!"));
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}
	
}

