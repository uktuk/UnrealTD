// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TD.h"
#include "TDPlayerController.h"
#include "TDPlayerPawn.h"
#include "AI/Navigation/NavigationSystem.h"

ATDPlayerController::ATDPlayerController()
{
	bShowMouseCursor = true;
	// DefaultMouseCursor = EMouseCursor::Crosshairs;
	IgnoreMoveInput = true;
	bIsSelectingTower = false;

	// setup User's UI Widgets
	if (WidgetTemplate)
	{
		if (!WidgetInstance)
		{
			WidgetInstance = CreateWidget<UUserWidget>(this, WidgetTemplate);
		}
	}
}

void ATDPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (SelectedTower != nullptr)
	{		
		FVector WorldLocation;
		FVector WorldDirection;

		// Project our mouse position into world space
		if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			ATDPlayerPawn* playerPawn = Cast<ATDPlayerPawn>(GetControlledPawn());
			FVector TraceEndLocation = playerPawn->playerCamera->GetForwardVector() + (12000 * WorldDirection);

			// Data structure to hold the Parameters to customize the Trace search
			FCollisionQueryParams TraceParams(FName(TEXT("CameraTrace")), true, this);
			TraceParams.bTraceAsyncScene = true;		// Trace Asynchronously so as not to block the program when tracing
			TraceParams.AddIgnoredActor(this);			// Ignore its own actor in the trace

			FHitResult Hit(ForceInit);

			if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, TraceEndLocation, ECC_WorldStatic, TraceParams))
			{
				FVector hitLocation = Hit.Location;
				SelectedTower->SetActorLocation(hitLocation);
			}
		}
	}
}

void ATDPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();	

	InputComponent->BindAction("LeftClick", IE_Released, this, &ATDPlayerController::LeftClicked);
	InputComponent->BindAction("RightClick", IE_Released, this, &ATDPlayerController::RightClicked);
}

void ATDPlayerController::LeftClicked()
{
	if (SelectedTower)
	{
		ATDPlayerPawn* playerPawn = Cast<ATDPlayerPawn>(GetControlledPawn());
		playerPawn->PlaceTower(SelectedTower, SelectedTower->GetActorLocation());
		SelectedTower = nullptr;
	}
	else
	{
		// Trace under the mouse to see if there is anything selectable
	}
}

void ATDPlayerController::RightClicked()
{
	if (SelectedTower)
	{
		SelectedTower->Destroy();
		SelectedTower = nullptr;
	}
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
}

void ATDPlayerController::SelectTower(ETowerTypes TowerType)
{
	int32 numTowerClasses = TowerClasses.Num();
	if ( (numTowerClasses > 0) && ( ((int32)TowerType) < numTowerClasses) )
	{		
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SelectedTower = GetWorld()->SpawnActor<ATDTower>(TowerClasses[(int32)TowerType], SpawnInfo);
	}
}