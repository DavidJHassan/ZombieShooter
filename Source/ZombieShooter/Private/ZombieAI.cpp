// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieShooter.h"
#include "ZombieAI.h"
#include "ZombieShooterCharacter.h"
#include "UnrealEd.h"
#include "Engine.h"
#include "ActorEditorUtils.h"
#include "ZombieAI.h"


AZombieAI::AZombieAI(const class FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer){
	PrimaryActorTick.bCanEverTick = true;
}

void AZombieAI::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	float minDistance = (float)HUGE;
	AZombieShooterCharacter *closestPlayer = NULL;
	for (TActorIterator<AZombieShooterCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr){
		float dist = ActorItr->GetDistanceTo(GetCharacter());
		if (dist < minDistance){
			closestPlayer = (*ActorItr);
		}
	}
	if (closestPlayer != NULL){
		MoveToActor(closestPlayer);
	}
}