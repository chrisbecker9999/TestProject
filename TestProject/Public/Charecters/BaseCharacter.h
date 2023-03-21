// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "ParagonPhase/CharacterTypes.h"
#include "InputActionValue.h"
#include "HUD/TargetCursorWidget.h"
#include "BaseCharacter.generated.h"

class UAnimMontage;
class UInputAction;
class AWeapon;
class UWidgetComponent;
class UAttributeComponent;

UCLASS()
class TESTPROJECT_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;	
	virtual void SetTargetCursorVisibility(bool Enabled);

protected:
	
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void Attack();
	virtual void Die();
	virtual void AbilityOne();
	virtual void AbilityTwo();
	virtual void AbilityThree();
	virtual void UltimateAbility();
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);

	UFUNCTION(BlueprintCallable)
	void PlayFallToGroundSound();
	UFUNCTION(BlueprintCallable)
	void PlayFallToKneesSound();
	UFUNCTION(BlueprintCallable)
	void PlayDeathCrySound();
	void SpawnHitParticles(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	virtual int32 PlayAttackMontage(UAnimMontage* Montage, TArray<FName> Sections);
	virtual int32 PlayDeathMontage();
	void StopAttackMontage();
	void DisableCapsule();
	virtual bool CanAttack();
	bool IsAlive();
	virtual void FocusTarget();

	UPROPERTY(VisibleAnywhere)
		UWidgetComponent* TargetCursor;

	UFUNCTION(BlueprintCallable)
		FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
		FVector GetTranslationWarpTarget();

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UFUNCTION(BlueprintCallable)
		void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
		virtual void AttackEnd();

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
		UAttributeComponent* Attributes;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* TwoHandAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* SpellAttackMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> SpellAttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> TwoHandAttackMontageSections;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* AbilityOneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* AbilityTwoAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* AbilityThreeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* UltimateAbilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* FocusTargetAction;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> FirstAbility;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> SecondAbility;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> ThirdAbility;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> Ultimate;

	UPROPERTY(BlueprintReadOnly)
		AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = Combat)
		double WarpTargetDistance = 75.f;

	UPROPERTY(BlueprintReadOnly)
	AActor* SelectedTarget;

private:

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* HitGroundSound;

	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* FallToKneesSound;

	UPROPERTY(EditAnywhere, Category = Combat)
		UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> DeathMontageSections;

};
