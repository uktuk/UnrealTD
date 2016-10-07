// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Slate.h"
#include <TDTower.h>
#include "TDPlayerController.generated.h"


UCLASS()
class ATDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDPlayerController();

protected:

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	// Class of widget to instantiate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSubclassOf<class UUserWidget> WidgetTemplate;

	// The Current instance of the widget UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		UUserWidget* WidgetInstance;

	// Class of widget to instantiate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSubclassOf<class UUserWidget> TowerSelectWidgetTemplate;

	// The Current instance of the widget UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		UUserWidget* TowerSelectInstance;

	// Array of Tower classes available for the player to spawn
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class ATDTower> > TowerClasses;
};