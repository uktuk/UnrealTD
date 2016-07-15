// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "TDPlayerCamera.h"
#include "TDGrid.h"
#include "TDTower.h"
#include "TDPlayerPawn.generated.h"

// Types of Towers
UENUM(BlueprintType)
enum class ETowerTypes : uint8
{
	TT_Tower1	UMETA(DisplayName = "Tower 1")
};

UCLASS()
class TD_API ATDPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATDPlayerPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Called when left click event fires
	void LeftClicked();

	// Called when right click event fires
	void RightClicked();

	// Moves PawnBox Forward and Backwards which pulls the CameraBoom
	void MoveForward(float val);

	// Moves PawnBox Right and Left which pulls the CameraBoom
	void MoveRight(float val);

	void OnZoomIn();

	void OnZoomOut();

	void PlaceTower(ATDTower* tower, FVector location);

	// Function to buy a tower from the menu	 
	// If you have the funds, puts a placeable tower on your cursor ready to be placed
	UFUNCTION(BlueprintCallable, Category = "Tower")
	void SelectTowerFromStore(ETowerTypes TowerType);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float moveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float zoomSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Root)
	UBoxComponent* pawnBox;

	 UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTDPlayerCamera* playerCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* cameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	ATDGrid* Grid;

	// Pointer to the currently selected tower
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	ATDTower* selectedTower;

	// Array of Tower Classes available for the player to buy and spawn
	UPROPERTY(EditAnywhere, Category = "Tower")
	TArray<TSubclassOf<class ATDTower> > towerClasses;

	// Array of Tower instances that have been placed in the world by the player
	UPROPERTY(EditAnywhere, Category = Tower)
	TArray<ATDTower*> towerList;
};