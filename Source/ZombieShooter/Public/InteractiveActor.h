// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

UCLASS()
class ZOMBIESHOOTER_API AInteractiveActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveActor(const FObjectInitializer& ObjectInitializer);

	///Whether or not the player is close enough to interact with the actor
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool bPlayerInRange;

	///Whether or not the local player is currently mousing over the pickup
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool bTargetedByPlayer;

	///Behaviour for the actor to perform upon player interaction
	UFUNCTION(BlueprintCallable, Category = Interaction)
	virtual void Interact(AActor* InteractingPlayer);

protected:
	///Sphere component used to check if the player is within range
	UPROPERTY(VisibleDefaultsOnly, Category = Pickup)
	class USphereComponent* InteractionVolume;

	///Static mesh to be visible within the game
	UPROPERTY(VisibleDefaultsOnly, Category = Pickup)
	class UStaticMeshComponent* Mesh;

	/*! Called when pickup beings overlap with another actor
	*	\param Other Actor that the pickup is overlapping
	*	\sa OnEndOverlap()
	*/
	UFUNCTION()
	virtual void OnBeginOverlap(AActor* OtherActor);

	/*! Called when pickup beings overlap with another actor
	*	\param Other Actor that the pickup is overlapping
	*	\sa OnBeginOverlap()
	*/
	UFUNCTION()
	virtual void OnEndOverlap(AActor* OtherActor);

	/*! Called when the local player first places their cursor
	*	overtop the object.
	*	\sa OnEndCursorOverlap()
	*/
	UFUNCTION()
	virtual void OnBeginCursorOverlap();

	/*! Called when the local player removes their cursor from
	*	the pickup.
	*/
	UFUNCTION()
	virtual void OnEndCursorOverlap();


};
