/*!\brief Pickup class
 *
 *	In world representation of items that the player is able to collect.
 */
#pragma once

#include "InteractiveActor.h"
#include "Pickup.generated.h"

UCLASS()
class ZOMBIESHOOTER_API APickup : public AInteractiveActor
{
	GENERATED_BODY()

	///Text to display the name of the item in the game
	UPROPERTY(VisibleDefaultsOnly, Category = Pickup)
	class UTextRenderComponent* ItemNameText;

public:
	APickup(const FObjectInitializer& ObjectInitializer);

	/*!\brief The name of the item
	 * 
	 *Temporary while no item class exists.
	 *Once item class created, will replace with reference to item object
	 */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
	FString ItemName;

	///The description of the item
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
	FString ItemDescription;

	/*! Initializes values at start of game.
	*/
	virtual void BeginPlay() override;

	/*! Handles interacting with the pickup
	* 
	*	Ensures that the player is in range, then attempts to collect the pickup
	*/
	virtual void Interact(AActor* InteractingPlayer) override;

protected:
	///Display interaction prompt
	virtual void OnBeginOverlap(AActor* OtherActor) override;

	///Hides interaction prompt
	virtual void OnEndOverlap(AActor* OtherActor) override;

	/*! Called when the local player first places their cursor
	*	overtop the pickup. Handles pickup targeting and name visibility.
	*	\sa OnEndCursorOverlap()
	*/
	virtual void OnBeginCursorOverlap() override;

	/*! Called when the local player removes their cursor from
	*	the pickup. Handles pickup targeting and name visibility.
	*	\sa OnBeginCursorOverlap()
	*/
	virtual void OnEndCursorOverlap() override;

	/*! Used when the item is collected by a player from the world.
	*
	*	When the item is collected, need to ensure that it is no longer in the world.
	*
	*	Once item class is created, will return reference to the collected item.
	*	For now, just return the name of the item
	*/
	UFUNCTION()
	FString Collect(AActor* Collector);
};
