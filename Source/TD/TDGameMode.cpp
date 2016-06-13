// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TD.h"
#include "TDGameMode.h"
#include "TDPlayerController.h"

ATDGameMode::ATDGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// use our custom PlayerController class
	ConstructorHelpers::FObjectFinder<UBlueprint> PlayerControllerBlueprint(TEXT("Blueprint'/Game/Player/PlayerController.PlayerController'"));

	if (PlayerControllerBlueprint.Object)
	{
		PlayerControllerClass = Cast<UClass>(PlayerControllerBlueprint.Object->GeneratedClass);

	}

	ConstructorHelpers::FObjectFinder<UBlueprint> PlayerBlueprint(TEXT("Blueprint'/Game/Player/PlayerPawn.PlayerPawn'"));

	if (PlayerBlueprint.Object)
	{
		DefaultPawnClass = Cast<UClass>(PlayerBlueprint.Object->GeneratedClass);
	}
}