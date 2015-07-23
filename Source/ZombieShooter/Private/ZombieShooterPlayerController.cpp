// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ZombieShooter.h"
#include "ZombieShooterPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Engine.h"

AZombieShooterPlayerController::AZombieShooterPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bRotating = false;
	CameraSensitivity = 0.5f;
	DefaultMouseCursor = EMouseCursor::Crosshairs;	
}

void AZombieShooterPlayerController::BeginPlayingState()
{
	ControlledPawn = Cast<AZombieShooterCharacter>(GetPawn());
}

void AZombieShooterPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (ControlledPawn)
	{
		FVector WorldLocation, WorldDirection;
		DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		ControlledPawn->WorldLocation = WorldLocation;
		ControlledPawn->WorldDirection = WorldDirection;
	}
	
	float MouseX = 0.0f, MouseY = 0.0f;
	GetMousePosition(MouseX, MouseY);
	
	if (bRotating)
	{
		RotateCameraRight((MouseX - LastMouseX) * CameraSensitivity);
		LastMouseX = MouseX;
	}
}

void AZombieShooterPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Fire", IE_Pressed, this, &AZombieShooterPlayerController::FirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AZombieShooterPlayerController::FireReleased);

	InputComponent->BindAction("DebugKey", IE_Pressed, this, &AZombieShooterPlayerController::DebugPressed);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AZombieShooterPlayerController::JumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &AZombieShooterPlayerController::JumpReleased);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AZombieShooterPlayerController::SprintPressed);
	InputComponent->BindAction("Sprint", IE_Released, this, &AZombieShooterPlayerController::SprintReleased);

	InputComponent->BindAction("Walk", IE_Pressed, this, &AZombieShooterPlayerController::WalkPressed);
	InputComponent->BindAction("Walk", IE_Released, this, &AZombieShooterPlayerController::WalkReleased);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &AZombieShooterPlayerController::CrouchPressed);
	InputComponent->BindAction("Crouch", IE_Released, this, &AZombieShooterPlayerController::CrouchReleased);

	InputComponent->BindAction("Prone", IE_Pressed, this, &AZombieShooterPlayerController::PronePressed);
	InputComponent->BindAction("Prone", IE_Released, this, &AZombieShooterPlayerController::ProneReleased);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AZombieShooterPlayerController::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AZombieShooterPlayerController::ZoomOut);

	InputComponent->BindAction("WeaponOne", IE_Pressed, this, &AZombieShooterPlayerController::WeaponOne);
	InputComponent->BindAction("WeaponTwo", IE_Pressed, this, &AZombieShooterPlayerController::WeaponTwo);
	InputComponent->BindAction("WeaponThree", IE_Pressed, this, &AZombieShooterPlayerController::WeaponThree);
	InputComponent->BindAction("WeaponFour", IE_Pressed, this, &AZombieShooterPlayerController::WeaponFour);

	InputComponent->BindAction("RotateCamera", IE_Pressed, this, &AZombieShooterPlayerController::RotateCameraPressed);
	InputComponent->BindAction("RotateCamera", IE_Released, this, &AZombieShooterPlayerController::RotateCameraReleased);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AZombieShooterPlayerController::InteractPressed);

	InputComponent->BindAxis("MoveUp", this, &AZombieShooterPlayerController::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &AZombieShooterPlayerController::MoveRight);
	InputComponent->BindAxis("RotateCameraRight", this, &AZombieShooterPlayerController::RotateCameraRight);
}

void AZombieShooterPlayerController::FirePressed()
{
	if (ControlledPawn)
	{
		ControlledPawn->bAttacking = true;
	}
}

void AZombieShooterPlayerController::FireReleased()
{
	if (ControlledPawn)
	{
		ControlledPawn->bAttacking = false;
	}
}

void AZombieShooterPlayerController::JumpPressed()
{
	if (ControlledPawn)
	{
		ControlledPawn->bPressedJump = true;
	}
}

void AZombieShooterPlayerController::JumpReleased()
{
	if (ControlledPawn)
	{
		ControlledPawn->bPressedJump = false;
	}
}

void AZombieShooterPlayerController::SprintPressed()
{
	if (ControlledPawn)
	{
		ControlledPawn->StartSprinting();
	}
}

void AZombieShooterPlayerController::SprintReleased()
{
	if (ControlledPawn)
	{
		ControlledPawn->StopSprinting();
	}
}

void AZombieShooterPlayerController::CrouchPressed()
{
	if (ControlledPawn)
	{
		ControlledPawn->Crouch();
	}
}

void AZombieShooterPlayerController::CrouchReleased()
{
	if (ControlledPawn)
	{
		ControlledPawn->UnCrouch();
	}
}

void AZombieShooterPlayerController::PronePressed()
{
	if (ControlledPawn)
	{
		ControlledPawn->bPressedProne = true;
	}
}

void AZombieShooterPlayerController::ProneReleased()
{
	if (ControlledPawn)
	{
		ControlledPawn->bPressedProne = false;
	}
}

void AZombieShooterPlayerController::WalkPressed()
{
	if (ControlledPawn)
	{
		ControlledPawn->StartWalking();
	}
}

void AZombieShooterPlayerController::WalkReleased()
{
	if (ControlledPawn)
	{
		ControlledPawn->StopWalking();
	}
}

void AZombieShooterPlayerController::DebugPressed()
{
	GetPawn()->TakeDamage(25, FDamageEvent(), this, GetPawn());
}

void AZombieShooterPlayerController::MoveUp(float Value)
{
	if (Value != 0.0f && !ControlledPawn->bPressedProne)
	{
		if (ControlledPawn)
		{
			ControlledPawn->MoveUp(Value);
		}
	}
}

void AZombieShooterPlayerController::MoveRight(float Value)
{
	if (Value != 0.0f && !ControlledPawn->bPressedProne)
	{
		if (ControlledPawn)
		{
			ControlledPawn->MoveRight(Value);
		}
	}
}

void AZombieShooterPlayerController::ZoomIn()
{
	if (ControlledPawn)
	{
		ControlledPawn->ZoomCamera(-50.0f);
	}
}

void AZombieShooterPlayerController::ZoomOut()
{
	if (ControlledPawn)
	{
		ControlledPawn->ZoomCamera(50.0f);
	}
}

void AZombieShooterPlayerController::RotateCameraPressed()
{
	GetMousePosition(InitialMouseX, InitialMouseY);
	LastMouseX = InitialMouseX;
	bRotating = true;
}

void AZombieShooterPlayerController::RotateCameraReleased()
{
	bRotating = false;
}

void AZombieShooterPlayerController::WeaponOne()
{
	if (ControlledPawn)
	{
		ControlledPawn->WeaponSwitch(1);
	}
}

void AZombieShooterPlayerController::WeaponTwo()
{
	if (ControlledPawn)
	{
		ControlledPawn->WeaponSwitch(2);
	}
}

void AZombieShooterPlayerController::WeaponThree()
{
	if (ControlledPawn)
	{
		ControlledPawn->WeaponSwitch(3);
	}
}

void AZombieShooterPlayerController::WeaponFour()
{
	if (ControlledPawn)
	{
		ControlledPawn->WeaponSwitch(4);
	}
}

void AZombieShooterPlayerController::InteractPressed()
{
	if (ControlledPawn)
	{
		ControlledPawn->Interact();
	}
}

void AZombieShooterPlayerController::RotateCameraRight(float Value)
{
	if (Value != 0.0f)
	{
		if (ControlledPawn)
		{
			ControlledPawn->RotateCamera(Value);
		}
	}
}