// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "ZombieShooterGameMode.generated.h"

UCLASS(minimalapi)
class AZombieShooterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AZombieShooterGameMode(const FObjectInitializer& ObjectInitializer);
	APlayerController* Login(UPlayer* NewPlayer, const FString& Portal, const FString& Options, const TSharedPtr<FUniqueNetId>& UniqueId, FString& ErrorMessage);

	UPROPERTY(EditAnywhere, NoClear, BlueprintReadWrite, Category = Classes)
	TSubclassOf< class APlayerController > OverlordControllerClass;

private:
	bool bAssignedOverlord;
};



