// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieShooter.h"
#include "Zombie2.h"


AZombie2::AZombie2(const FObjectInitializer& ObjectInitializer)
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
	
	//static ConstructorHelpers::FClassFinder<USkeletalMeshComponent> meshClass(TEXT("/Game/Character/HeroTPP"));
	//Mesh = meshClass.Class.GetDefaultObject;


	SetReplicates(true);
}


