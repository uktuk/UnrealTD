// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include <Engine.h>
#include "TDPlayerPawn.h"


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

	moveSpeed = 350.0f;
	zoomSpeed = 1200.0f;
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

}

// Called to bind functionality to input
void ATDPlayerPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ATDPlayerPawn::OnZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ATDPlayerPawn::OnZoomOut);

	// test for grid
	InputComponent->BindAction("PlaceTower", IE_Pressed, this, &ATDPlayerPawn::PlaceTower);

	InputComponent->BindAxis("MoveForward", this, &ATDPlayerPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATDPlayerPawn::MoveRight);
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

void ATDPlayerPawn::PlaceTower()
{
	UE_LOG(LogTemp, Warning, TEXT("Attempting to place tower"));
	if (Grid != nullptr && selectedTower != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			FVector WorldLocation;
			FVector WorldDirection;

			// Project our mouse position into world space
			if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
			{
				UE_LOG(LogTemp, Warning, TEXT("Deprojecting mouse position"));

				const FName tracetag("TraceTag");
				GetWorld()->DebugDrawTraceTag = tracetag;
				FVector TraceEndLocation = playerCamera->GetForwardVector() + (12000 * WorldDirection);

				// Data structure to hold the Parameters to customize the Trace search
				FCollisionQueryParams TraceParams(FName(TEXT("CameraTrace")), true, this);
				TraceParams.bTraceAsyncScene = true;		// Trace Asynchronously so as not to block the program when tracing
				TraceParams.AddIgnoredActor(this);			// Ignore its own actor in the trace
				TraceParams.TraceTag = tracetag;

				FHitResult Hit(ForceInit);

				UE_LOG(LogTemp, Warning, TEXT("Tracing world"));
				if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, TraceEndLocation, ECC_WorldStatic, TraceParams))
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit something in trace"));
					// Hit something (terrain), find closest node and place tower if placeable
					FVector hitLocation = Hit.Location;
					
					FTDTile* selectedTile = Grid->GetTileFromXY(hitLocation.X, hitLocation.Y);
					UE_LOG(LogTemp, Warning, TEXT("HitPosition = %f %f %f"), Hit.Location.X, Hit.Location.Y, Hit.Location.Z);
					UE_LOG(LogTemp, Warning, TEXT("SelectedTilePos = %f %f %f"), selectedTile->position.X, selectedTile->position.Y, selectedTile->position.Z);
					

					// If we found a tile and its not occupied
					if ((TowerClasses.Num() != 0) && (selectedTile != nullptr) && (selectedTile->bIsPlaceable))
					{
						// Always spawn the tower even if it's colliding
						FActorSpawnParameters SpawnInfo;						
						SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						ATDTower* NewTower = GetWorld()->SpawnActor<ATDTower>(TowerClasses[0], SpawnInfo);
						NewTower->SetActorLocation(selectedTile->position);
						NewTower->bHasBeenPlaced = true;
						towerList.Add(NewTower);
						selectedTile->bIsOccupied = true;
						selectedTile->bIsPlaceable = false;
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error: Player Controller == null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Grid == null"));
	}
}


void ATDPlayerPawn::PlaceTower(ATDTower* tower, FVector location)
{
	if (Grid && tower)
	{
		FTDTile* selectedTile = Grid->GetTileFromXY(location.X, location.Y);

		// If we found a tile and its not occupied
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