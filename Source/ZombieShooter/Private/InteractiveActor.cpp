// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieShooter.h"
#include "InteractiveActor.h"
#include "ZombieShooterPlayerController.h"

AInteractiveActor::AInteractiveActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InteractionVolume = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	InteractionVolume->InitSphereRadius(200.f);

	RootComponent = InteractionVolume;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));
	Mesh->AttachTo(RootComponent);

	OnActorBeginOverlap.AddDynamic(this, &AInteractiveActor::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AInteractiveActor::OnEndOverlap);

	OnBeginCursorOver.AddDynamic(this, &AInteractiveActor::OnBeginCursorOverlap);
	OnEndCursorOver.AddDynamic(this, &AInteractiveActor::OnEndCursorOverlap);

	bTargetedByPlayer = false;
}

void AInteractiveActor::Interact(AActor* InteractingPlayer){}

void AInteractiveActor::OnBeginOverlap(AActor* OtherActor)
{
	if (Cast<AZombieShooterCharacter>(OtherActor))
	{
		bPlayerInRange = true;
	}
}

void AInteractiveActor::OnEndOverlap(AActor* OtherActor)
{
	if (Cast<AZombieShooterCharacter>(OtherActor))
	{
		bPlayerInRange = false;
	}
}

void AInteractiveActor::OnBeginCursorOverlap()
{
	UWorld* World = GetWorld();
	if (World)
	{
		AZombieShooterPlayerController*  PlayerController = Cast<AZombieShooterPlayerController>(World->GetFirstPlayerController());
		if (PlayerController)
		{
			if (PlayerController->ControlledPawn)
			{
				PlayerController->ControlledPawn->TargetedObject = this;
				bTargetedByPlayer = true;
			}
		}
	}
}

void AInteractiveActor::OnEndCursorOverlap()
{
	UWorld* World = GetWorld();
	if (World)
	{
		AZombieShooterPlayerController*  PlayerController = Cast<AZombieShooterPlayerController>(World->GetFirstPlayerController());
		if (PlayerController)
		{
			if (PlayerController->ControlledPawn)
			{
				if (PlayerController->ControlledPawn->TargetedObject == this)
				{
					PlayerController->ControlledPawn->TargetedObject = NULL;
				}
				bTargetedByPlayer = false;
			}
		}
	}
}