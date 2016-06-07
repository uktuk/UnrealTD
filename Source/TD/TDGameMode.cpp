// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TD.h"
#include "TDGameMode.h"
#include "TDPlayerController.h"

ATDGameMode::ATDGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = ATDPlayerController::StaticClass();	

	ConstructorHelpers::FObjectFinder<UBlueprint> PlayerBlueprint(TEXT("Blueprint'/Game/Camera/PlayerPawn.PlayerPawn'"));

	if (PlayerBlueprint.Object)
	{
		DefaultPawnClass = Cast<UClass>(PlayerBlueprint.Object->GeneratedClass);
	}
}