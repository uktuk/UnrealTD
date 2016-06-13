// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Slate.h"
#include <TDTower.h>
#include "TDPlayerController.generated.h"

UENUM(BlueprintType)
enum class ETowerTypes : uint8
{
	TT_Tower1	UMETA(DisplayName = "Tower 1")
};


UCLASS()
class ATDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDPlayerController();

	UFUNCTION(BlueprintCallable, Category = "Tower")
	void SelectTower(ETowerTypes TowerType);

	// tower selected when player picks a tower from the spawn menu
	UPROPERTY(EditAnywhere)
	ATDTower* SelectedTower;

protected:

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void LeftClicked();
	void RightClicked();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

protected:

	// Class of widget to instantiate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> WidgetTemplate;
	
	// The Current instance of the widget UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UUserWidget* WidgetInstance;

	// Array of Tower classes available for the player to spawn
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ATDTower> > TowerClasses;

	// // Array of Tower Names to correspond with the TowerClasses
	// UPROPERTY(EditAnywhere)
	// ETowerTypes TowerTypes;

protected:

	UPROPERTY()
	bool bIsSelectingTower;
};


