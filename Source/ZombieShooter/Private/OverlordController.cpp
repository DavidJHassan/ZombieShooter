// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieShooter.h"
#include "OverlordController.h"
#include "Overlord.h"
#include "AI/Navigation/NavigationSystem.h"

AOverlordController::AOverlordController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AOverlordController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void AOverlordController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AOverlordController::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &AOverlordController::OnMoveRight);

	InputComponent->BindAction("PlaceTower", EInputEvent::IE_Pressed, this, &AOverlordController::placeTower);

}

void AOverlordController::OnMoveRight(float value){
	APawn* const pawn = GetPawn();

	if (value != 0.0f && pawn != NULL)
	{

		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		pawn->AddMovementInput(Direction, value);
	}
}

void AOverlordController::OnMoveForward(float value){
	APawn* const pawn = GetPawn();

	if (value != 0.0f && pawn != NULL)
	{

		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		pawn->AddMovementInput(Direction, value);
	}
}

void AOverlordController::placeTower()
{

	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		AOverlord* overlord = (AOverlord*)GetPawn();
		overlord->spawnTower(Hit.ImpactPoint);
	}
}
