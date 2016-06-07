// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
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
};


