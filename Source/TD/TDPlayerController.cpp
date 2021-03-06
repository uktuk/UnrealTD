// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TD.h"
#include "TDPlayerController.h"
#include "TDPlayerPawn.h"
// #include "AI/Navigation/NavigationSystem.h"

ATDPlayerController::ATDPlayerController()
{
	bShowMouseCursor = true;
	// DefaultMouseCursor = EMouseCursor::Crosshairs;
	IgnoreMoveInput = true;
}

void ATDPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ATDPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
}

void ATDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetTemplate)
	{
		if (!WidgetInstance)
		{
			WidgetInstance = CreateWidget<UUserWidget>(this, WidgetTemplate);
		}
	}

	if ((WidgetInstance) && (!WidgetInstance->GetIsVisible()))
	{
		WidgetInstance->AddToViewport();
	}

	// -------------------------------------------------------------------------

	if (TowerSelectWidgetTemplate)
	{
		if (!TowerSelectInstance)
		{
			TowerSelectInstance = CreateWidget<UUserWidget>(this, TowerSelectWidgetTemplate);
		}
	}

	if ((TowerSelectInstance) && (!TowerSelectInstance->GetIsVisible()))
	{
		TowerSelectInstance->AddToViewport();
	}
}