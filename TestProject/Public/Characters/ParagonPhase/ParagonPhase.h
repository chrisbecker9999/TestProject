// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputActionValue.h"
#include "Interfaces/PickupInterface.h"
#include "ParagonPhase.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class UPhaseOverlay;
class ASoul;
class ATreasure;
class AHealthPotion;
//class UGroomComponent;


UCLASS()
class TESTPROJECT_API AParagonPhase : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	
	AParagonPhase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void SetSelectedTarget(AActor* Target);
	AActor* GetSelectedTarget();
	
	FORCEINLINE virtual void GetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual void AddHealth(AHealthPotion* Health) override;

protected:
	
	virtual void BeginPlay() override;
	bool GetEnemysInRange();
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	virtual void Attack() override;
	virtual void AbilityOne() override;
	virtual void AbilityTwo() override;
	virtual void AbilityThree() override;
	virtual void UltimateAbility() override;
	void EKeyPressed();
	void Dodge();
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	void PlayEquipMontage(const FName& SectionName);
	virtual void Die() override;
	bool HasEnoughStamina();
	bool IsOccupied();
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	virtual void FocusTarget() override;
	virtual FVector GetRotationWarpTarget(AActor* Actor) override;
	virtual FVector GetTranslationWarpTarget(AActor* Actor) override;

	UFUNCTION(BlueprintCallable)
		void AttachWeaponToBack();
	UFUNCTION(BlueprintCallable)
		void AttachWeaponToHand();
	UFUNCTION(BlueprintCallable)
		void FinishEquipping();
	UFUNCTION(BlueprintCallable)
		void HitReactEnd();

	TArray<AActor*> OutActors;
	float SphereSize = 3000.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreActors;
	TArray<AActor*> PreviouslySelectedEnemys;
	TArray<AActor*> FoundEnemys;

	//TMap <AActor*, double> EnemyAndDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputMappingContext* PhaseContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* EKeyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* DodgeAction;

	UPROPERTY(BlueprintReadOnly)
		AActor* PhaseCombatTarget;

private:

	bool IsUnoccupied();
	void Initilize();
	void SetHUDHealth();

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY()
	UPhaseOverlay* PhaseOverlay;
	
};
