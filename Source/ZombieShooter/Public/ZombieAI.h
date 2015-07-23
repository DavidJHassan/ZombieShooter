// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "ZombieAI.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AZombieAI : public AAIController
{
	GENERATED_BODY()


	void Tick(float DeltaSeconds);
	AZombieAI(const class FObjectInitializer & ObjectInitializer);
};
