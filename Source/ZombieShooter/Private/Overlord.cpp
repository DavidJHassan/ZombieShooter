// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieShooter.h"
#include "Overlord.h"
#include "Spawner.h"

AOverlord::AOverlord(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

void AOverlord::spawnTower(FVector_NetQuantize location){
//	bool bHasNetOwner = HasNetOwner();
	if (Role < ROLE_Authority){
		ServerPlaceTower(location);
	}
//	else{
//		UWorld* const World = GetWorld();
//		if (World){
//			ASpawner* newActor = World->SpawnActor<ASpawner>(ASpawner::StaticClass(), location, (const FRotator)0, FActorSpawnParameters());
//		}
//	}
}

bool AOverlord::ServerPlaceTower_Validate(FVector_NetQuantize location){
	return true;
}

void AOverlord::ServerPlaceTower_Implementation(FVector_NetQuantize location){
	UWorld* const World = GetWorld();
	if (World){
		ASpawner* newActor = World->SpawnActor<ASpawner>(ASpawner::StaticClass(), location, (const FRotator)0, FActorSpawnParameters());
	}
}


