// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TDGrid.generated.h"

USTRUCT()
struct FTDTile
{	
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPlaceable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOccupied;

	FTDTile()
	{
		position = FVector(0.0f, 0.0f, 0.0f);
		bIsPlaceable = true;
		bIsOccupied = false;
	}

	FTDTile(FVector inPosition, bool inPlaceable = true, bool inOccupied = false)
	{
		position = inPosition;
		bIsPlaceable = inPlaceable;
		bIsOccupied = inOccupied;
	}
};

UCLASS()
class TD_API ATDGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDGrid();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void InitializeGrid();

	FTDTile* GetTileFromXY(const float x, const float y);

	// Width of the total map grid (usually the same width as terrain)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 numTilesWide;

	// Height of the total map grid (usually the same height as terrain)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 numTilesHigh;

	// Size of individual tiles in the grid
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 tileSize;
	
	TArray<FTDTile> tileList;
private:

	USceneComponent* SceneComponent;
};
