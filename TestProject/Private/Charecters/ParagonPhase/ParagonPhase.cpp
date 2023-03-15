// Fill out your copyright notice in the Description page of Project Settings.


#include "Charecters/ParagonPhase/ParagonPhase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"

//#include "GroomComponent.h"

// Sets default values
AParagonPhase::AParagonPhase()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 500.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
}

void AParagonPhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParagonPhase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//UE_LOG(LogTemp, Warning, TEXT("I made it inside ABird::SetupPlayerInputComponent IF statement!"));
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AParagonPhase::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AParagonPhase::Look);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &AParagonPhase::EKeyPressed);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AParagonPhase::Jump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AParagonPhase::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AParagonPhase::Dodge);
		EnhancedInputComponent->BindAction(AbilityOneAction, ETriggerEvent::Triggered, this, &AParagonPhase::AbilityOne);
		EnhancedInputComponent->BindAction(AbilityTwoAction, ETriggerEvent::Triggered, this, &AParagonPhase::AbilityTwo);
		EnhancedInputComponent->BindAction(AbilityThreeAction, ETriggerEvent::Triggered, this, &AParagonPhase::AbilityThree);
		EnhancedInputComponent->BindAction(UltimateAbilityAction, ETriggerEvent::Triggered, this, &AParagonPhase::UltimateAbility);
		EnhancedInputComponent->BindAction(FocusTargetAction, ETriggerEvent::Triggered, this, &AParagonPhase::FocusTarget);
	}

}

void AParagonPhase::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	ActionState = EActionState::EAS_HitReaction;
}

void AParagonPhase::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("EngageableTarget"));

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PhaseContext, 0);
		}
	}
	
}

void AParagonPhase::GetEnemysInRange()
{
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	IgnoreActors.Init(this, 1);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 
		SphereSize, ObjectTypes, nullptr, IgnoreActors, OutActors);

	for (AActor* overlappedActor : OutActors)
	{
	UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetName());
	UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------------------------------"));
	}

	//DrawDebugSphere(GetWorld(), GetActorLocation(), SphereSize, 12, FColor::Red, true, 10.0f);
}

void AParagonPhase::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void AParagonPhase::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AParagonPhase::Attack() 
{
	Super::Attack();

	if (CanAttack())
	{
		if (CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon)
		{
			PlayAttackMontage(AttackMontage, AttackMontageSections);
		}
		else if (CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon)
		{
			PlayAttackMontage(TwoHandAttackMontage, TwoHandAttackMontageSections);
		}
		else if (CharacterState == ECharacterState::ECS_EquippedSpell)
		{
			PlayAttackMontage(SpellAttackMontage, SpellAttackMontageSections);
		}
		
		ActionState = EActionState::EAS_Attacking;
	}
}

void AParagonPhase::AbilityOne()
{
	UWorld* World = GetWorld();
	CharacterState = ECharacterState::ECS_EquippedSpell;
	AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(FirstAbility);
	DefaultWeapon->Equip(GetMesh(), FName("SpellSocket"), this, this);
	EquippedWeapon = DefaultWeapon;
	//TODO
	//call function to detach sphere from hand socket
	//Have sphere travel to target
	//Play animation when it hits the enemy and destroy the sphere
	
}

void AParagonPhase::AbilityTwo()
{
	
}

void AParagonPhase::AbilityThree()
{
	
}

void AParagonPhase::UltimateAbility()
{
	
}

void AParagonPhase::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{		
		if (CanDisarm())
		{			
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

void AParagonPhase::Dodge()
{

}

void AParagonPhase::Jump()
{
	
		Super::Jump();
	

}

void AParagonPhase::EquipWeapon(AWeapon* Weapon)
{
	EquippedWeapon = Weapon;
		
	if (EquippedWeapon->GetWeaponType() == AWeaponType::AWT_OneHanded)
	{
		EquippedWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
	else if (EquippedWeapon->GetWeaponType() == AWeaponType::AWT_TwoHanded)
	{
		EquippedWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	}
	else if (EquippedWeapon->GetWeaponType() == AWeaponType::AWT_Spell)
	{
		EquippedWeapon->Equip(GetMesh(), FName("SpellSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedSpell;
	}
	OverlappingItem = nullptr;
}

void AParagonPhase::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
	AParagonPhase::GetCapsuleComponent()->SetWorldLocation(GetActorLocation());
}

bool AParagonPhase::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void AParagonPhase::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}
	
bool AParagonPhase::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied 
		&& CharacterState != ECharacterState::ECS_Unequipped;
}

bool AParagonPhase::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& CharacterState == ECharacterState::ECS_Unequipped
		&& EquippedWeapon;
}

void AParagonPhase::Disarm()
{
	ActionState = EActionState::EAS_UnEquippingWeapon;
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_Unoccupied;
}

void AParagonPhase::Arm()
{
	ActionState = EActionState::EAS_EquippingWeapon;

	PlayEquipMontage(FName("Equip"));
	if (EquippedWeapon->GetWeaponType() == AWeaponType::AWT_OneHanded)
	{
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
	else if (EquippedWeapon->GetWeaponType() == AWeaponType::AWT_TwoHanded)
	{
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	}

	ActionState = EActionState::EAS_Unoccupied;
}

void AParagonPhase::FocusTarget()
{
	GetEnemysInRange();

	if (CurrentSelection)
	{
		SetTargetCursorVisibility(false);
	}
	

	//int32 LoopCount = 0;

		for (AActor* A : OutActors)
		{
			//UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *A->GetName());
			//DistanceToTarget = A->GetActorLocation() - GetActorLocation();
			CurrentSelection = A;

			if (PreviouslySelected.Num() == OutActors.Num() && PreviouslySelected.Num() != 1 && OutActors.Num() != 1)
			{
				PreviouslySelected.Empty();
				//UE_LOG(LogTemp, Log, TEXT("List empied"));
			}
			else if (!PreviouslySelected.Contains(CurrentSelection))
			{
				SelectedTarget = CurrentSelection;
				DrawDebugSphere(GetWorld(), SelectedTarget->GetActorLocation(), 100, 12, FColor::MakeRandomColor(), true, 10.0f);
				PreviouslySelected.Add(SelectedTarget);
				SetTargetCursorVisibility(true);
				//UE_LOG(LogTemp, Log, TEXT("OutActors size: %i"), OutActors.Num());
				//UE_LOG(LogTemp, Log, TEXT("PreviouslySelected: %i"), PreviouslySelected.Num());
				return;
			}
			//LoopCount++;
			//UE_LOG(LogTemp, Log, TEXT("OutActors size: %i"), LoopCount);
		}	
}

void AParagonPhase::SetTargetCursorVisibility(bool Enabled)
{
	Super::SetTargetCursorVisibility(Enabled);
}

void AParagonPhase::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AParagonPhase::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AParagonPhase::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AParagonPhase::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
