// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include <Engine.h>
#include "TDGrid.h"


// Sets default values
ATDGrid::ATDGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	numTilesWide = 10;
	numTilesHigh = 10;
	tileSize = 100;
}

// Called when the game starts or when spawned
void ATDGrid::BeginPlay()
{
	Super::BeginPlay();
	InitializeGrid();
}

// Called every frame
void ATDGrid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
		
	int32 numberOfTiles = tileList.Num();
	if (numberOfTiles != 0)
	{
		for (int i = 0; i < numberOfTiles; ++i)
		{
			DrawDebugSphere(GetWorld(), tileList[i].position, 5, 4, FColor::Blue);
		}
	}
}

void ATDGrid::InitializeGrid()
{
	// Empty Tile list first in case we've already initialized
	tileList.Empty();

	// Position grid so it's offset by half its width & height (Causes SceneComponent to be in center of the grid)
	FVector gridOffset;
	gridOffset.X = SceneComponent->GetComponentLocation().X - ((tileSize * numTilesWide) / 2);
	gridOffset.Y = SceneComponent->GetComponentLocation().Y - ((tileSize * numTilesHigh) / 2);

	// Loop through Tiles and create tiles at coordinates and Z level with terrain by Tracing downwards
	for (int width = 0; width < numTilesWide; ++width)
	{
		for (int height = 0; height < numTilesHigh; ++height)
		{

			const FName tracetag("TraceTag");
			GetWorld()->DebugDrawTraceTag = tracetag;

			// Trace straight down at each coordinate
			FVector TraceStartLocation = FVector((width * tileSize) + gridOffset.X, (height * tileSize) + gridOffset.Y, 500.0f);
			FVector TraceEndLocation = FVector((width * tileSize) + gridOffset.X, (height * tileSize) + gridOffset.Y, -500.0f);
			
			// Data structure to hold the Parameters to customize the Trace search
			FCollisionQueryParams TraceParams(FName(TEXT("TileNodeTrace")), true, this);
			TraceParams.bTraceAsyncScene = true;		// Trace Asynchronously so as not to block the program when tracing
			TraceParams.AddIgnoredActor(this);			// Ignore its own actor in the trace
			TraceParams.TraceTag = tracetag;

			FHitResult Hit(ForceInit);


			float terrainZLevel = 500.0f;

			// Perform a Linetrace and store the result in Hit, Start, end, trace channel = our custom channel, and supply parameters we chose 
			if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStartLocation, TraceEndLocation, ECC_WorldStatic, TraceParams))
			{
				// Hit some terrain, set the node Z to the terrain Z (+ a little height)
				terrainZLevel = Hit.ImpactPoint.Z + 10.0f;
			}

			FTDTile newTile(FVector((width * tileSize) + gridOffset.X, (height * tileSize) + gridOffset.Y, terrainZLevel));
			tileList.Add(newTile);
		}
	}
}

FTDTile* ATDGrid::GetTileFromXY(const float inX, const float inY)
{
	// Position grid so it's offset by half its width & height (Causes SceneComponent to be in center of the grid)
	FVector gridOffset;
	gridOffset.X = SceneComponent->GetComponentLocation().X - ((tileSize * numTilesWide) / 2);
	gridOffset.Y = SceneComponent->GetComponentLocation().Y - ((tileSize * numTilesHigh) / 2);

	float x_new = round((inX - gridOffset.X) / float(tileSize));
	float y_new = round((inY - gridOffset.Y) / float(tileSize));

	int tileIndex = (x_new * numTilesHigh) + y_new;
	if (tileIndex > 0 && tileIndex < tileList.Num())
	{
		return &tileList[tileIndex];
	}
	else
	{
		return NULL;
	}
}