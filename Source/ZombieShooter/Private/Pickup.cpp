#include "ZombieShooter.h"
#include "Pickup.h"
#include "ZombieShooterCharacter.h"
#include "ZombieShooterPlayerController.h"

APickup::APickup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemNameText = ObjectInitializer.CreateDefaultSubobject<UTextRenderComponent>(this, TEXT("ItemNameText"));
	ItemNameText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	ItemNameText->WorldSize = 70.0f;
	ItemNameText->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	ItemNameText->AttachTo(Mesh);
	ItemNameText->SetVisibility(false);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	ItemNameText->Text = ItemName;
	//TODO: Change rotation logic to face local player's camera
	ItemNameText->SetWorldRotation(FRotator(50.0f, 180.0f, 0.0f));
}

FString APickup::Collect(AActor* Collector)
{
	UWorld* const World = GetWorld();
	if (World)
	{
		//TODO: Need network code here or somewhere along the line
		World->DestroyActor(this);
	}
	return ItemName;
}

void APickup::Interact(AActor* InteractingPlayer)
{
	Super::Interact(InteractingPlayer);
	if (bPlayerInRange)
	{
		Collect(InteractingPlayer);
	}
}

void APickup::OnBeginOverlap(AActor* OtherActor)
{
	Super::OnBeginOverlap(OtherActor);
	if (Cast<AZombieShooterCharacter>(OtherActor))
	{
		ItemNameText->SetText(ItemName + TEXT(" (F)"));
	}
}

void APickup::OnEndOverlap(AActor* OtherActor)
{	
	Super::OnEndOverlap(OtherActor);
	if (Cast<AZombieShooterCharacter>(OtherActor))
	{
		ItemNameText->SetText(ItemName);
	}
}

void APickup::OnBeginCursorOverlap()
{
	Super::OnBeginCursorOverlap();
	if (bTargetedByPlayer)
	{
		ItemNameText->SetVisibility(true);
	}
}

void APickup::OnEndCursorOverlap()
{
	Super::OnEndCursorOverlap();
	ItemNameText->SetVisibility(false);
}