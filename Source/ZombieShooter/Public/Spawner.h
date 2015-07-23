// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Zombie2.h"
#include "ZombieAI.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API ASpawner : public AActor
{
	GENERATED_BODY()
	
	public: 
		/** BoxComponent to specify the spawning area within the level */
		UPROPERTY(EditAnywhere, Category = Spawning)
		UBoxComponent* whereToSpawn;

		/** The pickup to spawn */
		UPROPERTY(EditAnywhere, Category = Spawning)
		TSubclassOf<class ACharacter> whatToSpawn;

		/** Minimum spawn delay */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		float spawnDelayLow;

		/** Maximum spawn delay */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		float spawnDelayHigh;

		/** Finds a random point within the BoxComponent */
		UFUNCTION(BlueprintPure, Category = Spawning)
		FVector GetRandomPointInVolume();

		virtual void Tick(float DeltaSeconds) override;

		ASpawner(const class FObjectInitializer& PCIP);

	private:
		bool spawnZombie;
		/** Handles the spawning of a new pickup */
		void spawn();

		float newSpawnDelay();

		/** The timer for when to spawn the pickup */
		float spawnTime;
		float spawnDelay;
	
	
};
