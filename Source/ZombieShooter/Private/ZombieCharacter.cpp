// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieShooter.h"
#include "ZombieCharacter.h"

AZombieCharacter::AZombieCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	PrimaryActorTick.bCanEverTick = true;

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


	MeleeAttackCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Melee Collision Box"));
	MeleeAttackCollision->AttachTo(RootComponent);
	MeleeAttackCollision->InitBoxExtent(FVector(60.0f, 50.0f, 100.0f));
	MeleeAttackCollision->SetRelativeLocation(FVector(50.0f, 0.0f, 20.0f));

	MaxHealth = 100;
	CurrentHealth = MaxHealth;

}

float AZombieCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Applying Damage!"));
	CurrentHealth -= ActualDamage;
	if (CurrentHealth <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Killing the zombie!"));



		UWorld* const World = GetWorld();
		if (World)
		{
			World->DestroyActor(this);
		}




	}
	return ActualDamage;
}