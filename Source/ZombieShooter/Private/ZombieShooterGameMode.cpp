// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ZombieShooter.h"
#include "ZombieShooterGameMode.h"
#include "ZombieShooterPlayerController.h"
#include "ZombieShooterCharacter.h"
#include "OverlordController.h"
#include "Overlord.h"

AZombieShooterGameMode::AZombieShooterGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
		bool isShooter = true;// TOGGLE this to switch between shooter or overlord mode

		if (isShooter)
		{
			PlayerControllerClass = AZombieShooterPlayerController::StaticClass();
			static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
			if (PlayerPawnBPClass.Class != NULL)
			{
				DefaultPawnClass = PlayerPawnBPClass.Class;
			}
		}
		else
		{
			PlayerControllerClass = AOverlordController::StaticClass();

			DefaultPawnClass = AOverlord::StaticClass();
			bAssignedOverlord = false;
		}
		//OverlordControllerClass = AOverlordController::StaticClass();
}

APlayerController* AZombieShooterGameMode::Login(UPlayer* NewPlayer, const FString& Portal, const FString& Options, const TSharedPtr<FUniqueNetId>& UniqueId, FString& ErrorMessage)
{
/*	if (!bAssignedOverlord){
		TSubclassOf< class APlayerController > temp  = PlayerControllerClass;
		PlayerControllerClass = OverlordControllerClass;
		APlayerController* NewPlayerController = Super::Login(NewPlayer, Portal, Options, UniqueId, ErrorMessage);
		PlayerControllerClass = temp;
		bAssignedOverlord = true;
		return NewPlayerController;
	}
	else{
*/		return Super::Login(NewPlayer, Portal, Options, UniqueId, ErrorMessage);
//	}
}