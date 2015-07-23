// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ZombieCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AZombieCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* MeleeAttackCollision;
	
public:
	AZombieCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	float MaxHealth;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Health)
	float CurrentHealth;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
};
