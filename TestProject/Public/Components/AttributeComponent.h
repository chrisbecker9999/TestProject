// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//Current health
	UPROPERTY(EditAnywhere, category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, category = "Actor Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, category = "Actor Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, category = "Actor Attributes")
	int32 Souls;

	UPROPERTY(EditAnywhere, category = "Actor Attributes")
		float DodgeCost = 20.f;

	UPROPERTY(EditAnywhere, category = "Actor Attributes")
		float StaminaRegenRate = 2.f;

	
public:
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 AmountOfGold);
	void AddHealth(int32 AmountOfHealth);
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE int32 GetHealth() const { return Health; }
};
