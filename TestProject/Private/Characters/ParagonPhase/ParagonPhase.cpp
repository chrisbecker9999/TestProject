// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ParagonPhase/ParagonPhase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/AttributeComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Enemy/Enemy.h"
#include "Perception/PawnSensingComponent.h"
#include "HUD/PhaseHUD.h"
#include "HUD/PhaseOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/HealthPotion.h"

//#include "GroomComponent.h"

// Sets default values
AParagonPhase::AParagonPhase()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	//UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3: %d"), AvgInt);
}

void AParagonPhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Attributes && PhaseOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		PhaseOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
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

void AParagonPhase::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

void AParagonPhase::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
	
}

float AParagonPhase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void AParagonPhase::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AParagonPhase::AddSouls(ASoul* Soul)
{
	if (Attributes && PhaseOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		PhaseOverlay->SetSouls(Attributes->GetSouls());
	}
}

void AParagonPhase::AddGold(ATreasure* Treasure)
{
	if (Attributes && PhaseOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		PhaseOverlay->SetGold(Attributes->GetGold());
	}
}

void AParagonPhase::AddHealth(AHealthPotion* Health)
{
	if (Attributes && PhaseOverlay)
	{
		Attributes->AddHealth(Health->GetHealth());
		PhaseOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void AParagonPhase::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("EngageableTarget"));

	Initilize();
}

bool AParagonPhase::GetEnemysInRange()
{
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	IgnoreActors.Init(this, 1);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 
		SphereSize, ObjectTypes, nullptr, IgnoreActors, OutActors);
	if (!OutActors.IsEmpty())
	{		
		return true;
	}
	else
	{
		return false;
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
	if (IsOccupied() || !HasEnoughStamina()) return;
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && PhaseOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		PhaseOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
	//UE_LOG(LogTemp, Warning, TEXT("Dodge function triggered!"));
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
		EquippedWeapon->Equip(GetMesh(), FName("TwoHandSocket"), this, this);
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

void AParagonPhase::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
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

void AParagonPhase::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

bool AParagonPhase::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

bool AParagonPhase::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
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
	if (GetEnemysInRange())
	{
		//Populate the array of previously found enemies
		FoundEnemys = OutActors;

		if (SelectedTarget)
		{
			TObjectPtr<AEnemy> E = Cast<AEnemy>(SelectedTarget);

			if (IsValid(E))
			{
				//Disable the target cursor for the last selected enemy
				E->SetTargetCursorVisibility(false);

				//Determine if all enenmys within range were previously targeted, if so empty the previously targeted array
				int32 Counter = 0;
				for (AActor* A : FoundEnemys)
				{
					if (PreviouslySelectedEnemys.Contains(A))
					{
						Counter++;
					}
				}

				if (Counter == FoundEnemys.Num())
				{
					PreviouslySelectedEnemys.Empty();
				}
				else if (PreviouslySelectedEnemys == FoundEnemys)
				{
					PreviouslySelectedEnemys.Empty();
				}

				//Iterate through the list of previously selected targets
				for (AActor* A : PreviouslySelectedEnemys)
				{					//Remove enemy that was selected last time
					if (FoundEnemys.Contains(A))
					{
						FoundEnemys.Remove(A);
					}
				}
			}
		}

		double EnemyRange = 0.0;
		double Min = DBL_MIN;
		double Max = DBL_MAX;

		TMap <AActor*, double> EnemyAndDistance;

		//Iterate through the list of enemies that are with in range
		for (AActor* A : FoundEnemys)
		{
			TObjectPtr<AEnemy> E = Cast<AEnemy>(A);

			EnemyRange = (GetActorLocation() - A->GetActorLocation()).Size();

			if (EnemyAndDistance.Contains(A))
			{
				EnemyAndDistance.Remove(A);
			}

			EnemyAndDistance.Emplace(A, EnemyRange);

		}
		//Iterate through the list of enemy distances to find which is the closest
		for (auto& Elem : EnemyAndDistance)
		{
			if (Elem.Value > Min) 
			{
				Max = Elem.Value;
			}
			else
			{
				Min = Elem.Value;
			}
		}
		
		//Set the current selected target
		for (AActor* A : FoundEnemys)
		{
			for (auto& Elem : EnemyAndDistance)
			{				
				//Find which enemy is the closest and select them
				if (EnemyAndDistance.Contains(A) && Elem.Value == Min || EnemyAndDistance.Contains(A) && Elem.Value == Max)
				{
					SelectedTarget = A;
					TObjectPtr<AEnemy> E = Cast<AEnemy>(A);

					if (IsValid(E))
					{
						//Enable the target cursor for the current selected enemy
						E->SetTargetCursorVisibility(true);
						PhaseCombatTarget = SelectedTarget;
						PreviouslySelectedEnemys.Emplace(SelectedTarget);
						return;
					}
				}				
			}
		}
	}
}

FVector AParagonPhase::GetRotationWarpTarget(AActor* Actor)
{
	return Super::GetRotationWarpTarget(Actor);
}

FVector AParagonPhase::GetTranslationWarpTarget(AActor* Actor)
{
	return Super::GetTranslationWarpTarget(Actor);
}

void AParagonPhase::SetSelectedTarget(AActor* Target)
{
	SelectedTarget = Target;
}

AActor* AParagonPhase::GetSelectedTarget()
{
	return SelectedTarget;
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

bool AParagonPhase::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void AParagonPhase::Initilize()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		APhaseHUD* PhaseHUD = Cast<APhaseHUD>(PlayerController->GetHUD());
		if (PhaseHUD)
		{
			PhaseOverlay = PhaseHUD->GetPhaseOverlay();
			if (PhaseOverlay && Attributes)
			{
				PhaseOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				PhaseOverlay->SetStaminaBarPercent(1.f);
				PhaseOverlay->SetGold(0);
				PhaseOverlay->SetSouls(0);
			}
		}

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PhaseContext, 0);
		}
	}
}

void AParagonPhase::SetHUDHealth()
{
	if (PhaseOverlay)
	{
		PhaseOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

