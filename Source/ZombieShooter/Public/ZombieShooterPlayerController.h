// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "ZombieShooterCharacter.h"
#include "ZombieShooterPlayerController.generated.h"

UCLASS()
class AZombieShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZombieShooterPlayerController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraSensitivity;

	UPROPERTY()
	bool bRotating;

	UPROPERTY()
	AZombieShooterCharacter* ControlledPawn;

protected:
	float InitialMouseX, InitialMouseY;
	float LastMouseX;
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlayingState() override;
	// End PlayerController interface

	/** Mouse click input handlers*/
	UFUNCTION()
	void FirePressed();

	UFUNCTION()
	void FireReleased();

	UFUNCTION()
	void DebugPressed();

	UFUNCTION()
	void JumpPressed();

	UFUNCTION()
	void JumpReleased();

	UFUNCTION()
	void SprintPressed();

	UFUNCTION()
	void SprintReleased();

	UFUNCTION()
	void WalkPressed();

	UFUNCTION()
	void WalkReleased();

	UFUNCTION()
	void ZoomIn();

	UFUNCTION()
	void ZoomOut();

	UFUNCTION()
	void RotateCameraPressed();

	UFUNCTION()
	void RotateCameraReleased();

	UFUNCTION()
	void WeaponOne();

	UFUNCTION()
	void WeaponTwo();

	UFUNCTION()
	void WeaponThree();

	UFUNCTION()
	void WeaponFour();

	UFUNCTION()
	void InteractPressed();

	UFUNCTION()
	void CrouchPressed();

	UFUNCTION()
	void CrouchReleased();

	UFUNCTION()
	void PronePressed();

	UFUNCTION()
	void ProneReleased();

	/** Input handlers for directional movement. */
	UFUNCTION()
	void MoveUp(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void RotateCameraRight(float Value);
};


