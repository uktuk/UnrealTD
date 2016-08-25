// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include <Engine.h>
#include "TDPlayerPawn.h"
#include "GameFramework/PlayerController.h"


// Sets default values
ATDPlayerPawn::ATDPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = pawnBox;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	cameraBoom->bEnableCameraLag = true;
	cameraBoom->CameraLagSpeed = 4.0f;
	cameraBoom->AttachTo(RootComponent);

	playerCamera = CreateDefaultSubobject<UTDPlayerCamera>("PlayerCamera");
	playerCamera->AttachTo(cameraBoom);

	Grid = nullptr;
	selectedTower = nullptr;

	moveSpeed = 500.0f;
	zoomSpeed = 4000.0f;
}

// Called when the game starts or when spawned
void ATDPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDPlayerPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (selectedTower != nullptr)
	{
	   
		// Player has selected a tower from the store and has not placed it yet
		if (!selectedTower->bHasBeenPlaced)
		{
			FVector WorldLocation;
			FVector WorldDirection;

			// Project our mouse position into world space
			APlayerController* playerController = Cast<APlayerController>(Controller);
			if (playerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
			{
				// ATDPlayerPawn* playerPawn = Cast<ATDPlayerPawn>(GetControlledPawn());
				FVector TraceEndLocation = playerCamera->GetForwardVector() + (12000 * WorldDirection);

				// Data structure to hold the Parameters to customize the Trace search
				FCollisionQueryParams TraceParams(FName(TEXT("CameraTrace")), true, this);
				TraceParams.bTraceAsyncScene = true;		// Trace Asynchronously so as not to block the program when tracing
				TraceParams.AddIgnoredActor(this);			// Ignore its own actor in the trace

				FHitResult Hit(ForceInit);

				if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, TraceEndLocation, ECC_WorldStatic, TraceParams))
				{
					FVector hitLocation = Hit.Location;
					FTDTile* hitTile = Grid->GetTileFromXY(Hit.Location.X, Hit.Location.Y);
					FVector distToNearestTile = hitTile->position - Hit.Location;

					if (distToNearestTile.SizeSquared() <= FMath::Square(30.0f))
					{
						selectedTower->SetActorLocation(hitTile->position);
					}
					else
					{
						selectedTower->SetActorLocation(Hit.Location);
					}
				}
			}
		}		
		else // Player has selected a tower which is already in place in the world
		{
			// show UI for tower (stats, abilities, sell, etc)
		}
	}
}

// Called to bind functionality to input
void ATDPlayerPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);

	InputComponent->BindAction("LeftClick", IE_Released, this, &ATDPlayerPawn::LeftClicked);
	InputComponent->BindAction("RightClick", IE_Released, this, &ATDPlayerPawn::RightClicked);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ATDPlayerPawn::OnZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ATDPlayerPawn::OnZoomOut);

	InputComponent->BindAxis("MoveForward", this, &ATDPlayerPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATDPlayerPawn::MoveRight);
}

void ATDPlayerPawn::LeftClicked()
{
	if (selectedTower && !selectedTower->bHasBeenPlaced)
	{
		PlaceTower(selectedTower, selectedTower->GetActorLocation());
		selectedTower = nullptr;
	}
}

void ATDPlayerPawn::RightClicked()
{	
	if (selectedTower)
	{
		if (!selectedTower->bHasBeenPlaced)
		{
			selectedTower->Destroy();
		}
		selectedTower = nullptr;
	}
}

void ATDPlayerPawn::MoveForward(float val)
{
	
	if (val != 0)
	{
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * (moveSpeed * val * GetWorld()->DeltaTimeSeconds)));
	}
}

void ATDPlayerPawn::MoveRight(float val)
{
	if (val != 0)
	{
		SetActorLocation(GetActorLocation() + (GetActorRightVector() * (moveSpeed * val * GetWorld()->DeltaTimeSeconds)));
	}
}

void ATDPlayerPawn::OnZoomIn()
{
	//TODO: Add smoothing to zoom in
	cameraBoom->TargetArmLength += (zoomSpeed * GetWorld()->DeltaTimeSeconds);
}

void ATDPlayerPawn::OnZoomOut()
{
	//TODO: Add smoothing to zoom out
	cameraBoom->TargetArmLength -= (zoomSpeed * GetWorld()->DeltaTimeSeconds);
}

void ATDPlayerPawn::PlaceTower(ATDTower* tower, FVector location)
{
	if (Grid && tower)
	{
		FTDTile* selectedTile = Grid->GetTileFromXY(location.X, location.Y);

		// If we found a tile and it's not occupied
		if ((selectedTile != nullptr) && (selectedTile->bIsPlaceable))
		{
			tower->SetActorLocation(selectedTile->position);
			tower->bHasBeenPlaced = true;
			towerList.Add(tower);
			selectedTile->bIsOccupied = true;
			selectedTile->bIsPlaceable = false;
		}
	}
}

void ATDPlayerPawn::SelectTowerFromStore(ETowerTypes TowerType)
{
	int32 numTowerClasses = towerClasses.Num();	
	if ((numTowerClasses > 0) && ((int32)TowerType < numTowerClasses) &&
		((selectedTower == nullptr) || (selectedTower != nullptr && selectedTower->bHasBeenPlaced)))  // If there is no tower selected OR the tower selected has already been placed
	{
		FActorSpawnParameters spawnInfo;
		spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		selectedTower = GetWorld()->SpawnActor<ATDTower>(towerClasses[(int32)TowerType], spawnInfo);
	}
}