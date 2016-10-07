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
	prePlacementTwr = nullptr;

	moveSpeed = 500.0f;
	zoomSpeed = 4000.0f;
}

// Called when the game starts or when spawned
void ATDPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATDPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (prePlacementTwr != nullptr)
	{
		// Player has selected a tower from the store and has not placed it yet
		FHitResult Hit(ForceInit);
		if (TraceAtMouseCursor(Hit, ECC_GameTraceChannel1, 5000.0f))
		{
			// Trace to mouse cursor and place the prePlacement tower at cursor location
			FVector hitLocation = Hit.Location;
			FTDTile* hitTile = Grid->GetTileFromXY(Hit.Location.X, Hit.Location.Y);
			if (hitTile)
			{
				FVector distToNearestTile = hitTile->position - Hit.Location;
		
				// Snap to grid
				if (distToNearestTile.SizeSquared() <= FMath::Square(30.0f))
				{
					prePlacementTwr->SetActorLocation(hitTile->position);
				}
				else
				{
					prePlacementTwr->SetActorLocation(Hit.Location);
				}
			}
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
	if (prePlacementTwr)
	{
		PlaceTower(prePlacementTwr, prePlacementTwr->GetActorLocation());
		selectedTower = prePlacementTwr;
		prePlacementTwr = nullptr;
	}

	FHitResult Hit(ForceInit);
	TraceAtMouseCursor(Hit, ECC_Pawn, 5000.0f, true);
	AActor* hitActor = Hit.Actor.Get();

	if (hitActor && hitActor->IsA(ATDTower::StaticClass()) && (hitActor != selectedTower))
	{
		if (selectedTower)
		{
			selectedTower->OnDeselected();
		}
		selectedTower = Cast<ATDTower>(hitActor);
		selectedTower->OnSelected();

	}
	else
	{
		if (selectedTower)
		{
			selectedTower->OnDeselected();
		}
		selectedTower = nullptr;
	}


}

void ATDPlayerPawn::RightClicked()
{
	if (prePlacementTwr)
	{
		prePlacementTwr->Destroy();
		prePlacementTwr = nullptr;
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
	if (prePlacementTwr)
	{
		prePlacementTwr->Destroy();
		prePlacementTwr = nullptr;
	}

	int32 numTowerClasses = towerClasses.Num();
	if ( (numTowerClasses > 0) && ((int32)TowerType < numTowerClasses) )
	{
		if (selectedTower)
		{
			selectedTower->OnDeselected();
			selectedTower = nullptr;
		}

		FActorSpawnParameters spawnInfo;
		spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		prePlacementTwr = GetWorld()->SpawnActor<ATDTower>(towerClasses[(int32)TowerType], spawnInfo);
		selectedTower = prePlacementTwr;
		selectedTower->OnSelected();
	}
}

bool ATDPlayerPawn::TraceAtMouseCursor(FHitResult& hitResult, ECollisionChannel collisionChannel, float distance, bool DrawDebug)
{
	bool didSucceed = false;
	hitResult.Init();

	FVector WorldLocation;
	FVector WorldDirection;

	// Project our mouse position into world space
	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (playerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// Data structure to hold the Parameters to customize the Trace search
		FCollisionQueryParams TraceParams(FName(TEXT("CameraTrace")), true, this);
		TraceParams.bTraceAsyncScene = true;		// Trace Asynchronously so as not to block the program when tracing
		TraceParams.AddIgnoredActor(this);			// Ignore its own actor in the trace				

		FVector traceEndLocation = WorldDirection;
		traceEndLocation.Normalize();
		traceEndLocation *= distance;
		traceEndLocation += playerCamera->GetComponentLocation();		

		if (DrawDebug)
		{
			DrawDebugLine(GetWorld(), WorldLocation, traceEndLocation, FColor::Red, true);
		}

		//if (GetWorld()->LineTraceSingleByObjectType(Hit, WorldLocation, TraceEndLocation, ECC_Visibility, TraceParams))
		if (GetWorld()->LineTraceSingleByChannel(hitResult, WorldLocation, traceEndLocation, collisionChannel, TraceParams))
		{
			return didSucceed = true;
		}
	}

	return didSucceed;
}