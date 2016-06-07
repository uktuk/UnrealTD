// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "TDPlayerCamera.h"
#include "TDGrid.h"
#include "TDTower.h"
#include "TDPlayerPawn.generated.h"

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

	// Moves PawnBox Forward and Backwards which pulls the CameraBoom
	void MoveForward(float val);

	// Moves PawnBox Right and Left which pulls the CameraBoom
	void MoveRight(float val);

	// Zooms in the camera
	UFUNCTION()
	void OnZoomIn();

	// Zooms out the camera
	UFUNCTION()
	void OnZoomOut();

	// Attempts to place a tower at mouse position
	UFUNCTION()
	void PlaceTower();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float moveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float zoomSpeed;

protected:

	// Box Component for the camera + springarm to attach to and follow
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Root)
	UBoxComponent* pawnBox;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTDPlayerCamera* playerCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* cameraBoom;

	// The grid consisting of Tiles to place towers on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	ATDGrid* Grid;

	// Which tower is currently selected
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	TSubclassOf<ATDTower> selectedTower;

	// List of classes which can be spawned
	UPROPERTY(EditAnywhere, Category=Tower)
	TArray<TSubclassOf<class ATDTower> > TowerClasses;

	// List of towers which have been placed by the player in the level
	UPROPERTY(EditAnywhere, Category = Tower)
	TArray<ATDTower*> towerList;
};