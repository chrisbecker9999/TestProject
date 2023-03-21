// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Charecters/BaseCharacter.h"
#include "Charecters/ParagonPhase/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class TESTPROJECT_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	
	//Ihit Interface
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter);
	//<AActor>
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	//</AActor>
	virtual void SetTargetCursorVisibility(bool Enabled) override;

protected:

	virtual void BeginPlay() override;
	void WaitForNavMeshLoad();
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
		EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:

	//AI Behavior
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsEngaged();
	bool IsAttacking();
	bool IsDead();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();

	UFUNCTION()
		virtual void PawnSeen(APawn* SeenPawn);

	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensing;

	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;

	UPROPERTY(VisibleAnywhere)
		UHealthBarComponent* HealthBarWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
		double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere)
		double AttackRadius = 150.f;

	UPROPERTY()
		class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathLifeSpan = 8.f;

};
