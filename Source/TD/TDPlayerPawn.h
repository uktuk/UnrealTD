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

	void OnZoomIn();

	void OnZoomOut();

	void PlaceTower();

	void PlaceTower(ATDTower* tower, FVector location);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	TSubclassOf<ATDTower> selectedTower;

	UPROPERTY(EditAnywhere, Category=Tower)
	TArray<TSubclassOf<class ATDTower> > TowerClasses;

	UPROPERTY(EditAnywhere, Category = Tower)
	TArray<ATDTower*> towerList;
};