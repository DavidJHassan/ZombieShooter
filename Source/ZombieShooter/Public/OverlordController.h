// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spawner.h"
#include "GameFramework/PlayerController.h"
#include "OverlordController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AOverlordController : public APlayerController
{
	GENERATED_BODY()
	

	public:
	AOverlordController(const FObjectInitializer& ObjectInitializer);

	protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void OnMoveRight(float value);
	void OnMoveForward(float value);
	void placeTower();
};
