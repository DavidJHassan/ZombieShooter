// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieShooter.h"
#include "Spawner.h"

ASpawner::ASpawner(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	// Create the simple StaticMeshComponent to represent the pickup in the level 
	whereToSpawn = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("WhereToSpawn"));

	// Set the StaticMeshComponent as the root component
	RootComponent = whereToSpawn;

	// Make the SpawnVolume tickable
	PrimaryActorTick.bCanEverTick = true;
	spawnTime = 0.0f;
	spawnDelayLow = 1.0f;
	spawnDelayHigh = 5.0f;
	spawnDelay = newSpawnDelay();
	spawnZombie = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<ACharacter> ZombieClass(TEXT("/Game/Blueprints/BasicZombie"));
	if (ZombieClass.Class != NULL)
	{
		whatToSpawn = ZombieClass.Class;
	}
}
void ASpawner::Tick(float deltaSeconds){
	spawnTime += deltaSeconds;

	if (spawnTime >= spawnDelay){
		spawnTime = 0.0f;
		spawnDelay = newSpawnDelay();
		spawn();
	}
}

float ASpawner::newSpawnDelay(){
	return FMath::FRandRange(spawnDelayLow, spawnDelayHigh);
}

void ASpawner::spawn(){
	if (whatToSpawn != NULL){
		UWorld* const World = GetWorld();
		if (World)
		{

			// Get a random location to spawn at
			FVector SpawnLocation = GetRandomPointInVolume();
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			// spawn the pickup
			ACharacter* newActor = World->SpawnActor<ACharacter>(whatToSpawn, SpawnLocation, (const FRotator)0, SpawnParams);
			if (newActor != NULL){
				newActor->SpawnDefaultController();
			}
		}
	}
}

FVector ASpawner::GetRandomPointInVolume(){
	
	FVector Origin;
	FVector BoxExtent;

	BoxExtent = whereToSpawn->Bounds.BoxExtent;
	Origin = whereToSpawn->Bounds.Origin;

	FVector dist = FVector(FMath::FRandRange(-BoxExtent.X / 2, BoxExtent.X / 2),
		FMath::FRandRange(-BoxExtent.Y / 2, BoxExtent.Y / 2),
		FMath::FRandRange(-BoxExtent.Z / 2, BoxExtent.Z / 2));

	return dist + Origin;
}