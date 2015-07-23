// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "InteractiveActor.h"
#include "Projectile.h"
#include "ZombieShooterCharacter.generated.h"

UCLASS(Blueprintable)
class AZombieShooterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	AZombieShooterCharacter(const FObjectInitializer& ObjectInitializer);

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable, Category = Attacks)
	void PerformAttack();

	UFUNCTION()
	void RotateCamera(float Value);

	UFUNCTION()
	void ZoomCamera(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void MoveUp(float Value);

	UFUNCTION()
	void CalculateFacing();

	UFUNCTION()
	void ChangeFacing(FRotator Rotation);

	UFUNCTION(reliable, server, WithValidation)
	void ServerChangeFacing(FRotator Rotation);

	UFUNCTION(reliable, server, WithValidation)
	void ServerPerformAttack();

	UFUNCTION()
	void StartSprinting();

	UFUNCTION()
	void StopSprinting();

	UFUNCTION()
	void StartWalking();

	UFUNCTION()
	void StopWalking();

	UFUNCTION()
	void WeaponSwitch(uint16 Value);

	UFUNCTION()
	void Interact();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Attacks)
	bool bAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bPressedProne;

	UPROPERTY()
	uint16 CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float MaxHealth;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Health)
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	float SprintSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	float RunSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	float WalkSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Movement)
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Replicated)
	AProjectile *Projectile;

	UPROPERTY()
	FVector WorldLocation;

	UPROPERTY()
	FVector WorldDirection;

	UPROPERTY()
	AInteractiveActor* TargetedObject;

protected:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attacks)
	float AttackCooldownBase;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attacks)
	float AttackCooldownRemaining;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float MaxCameraDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float MinCameraDistance;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;
};

