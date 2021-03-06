// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "TDPlayerCamera.generated.h"

/**
 * 
 */
UCLASS()
class TD_API UTDPlayerCamera : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	
	UTDPlayerCamera(const FObjectInitializer & ObjectInitializer);

	void OnZoomIn();

	void OnZoomOut();

	void UpdateCameraMovement();

	void MoveForward(float val);

	void MoveRight(float val);

	void SetZoomLevel(float newLevel);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera Settings")
	float MinZoomLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float MaxZoomLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float CameraScrollSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	FRotator FixedCameraAngle;


private:

	APawn* GetOwnerPawn();

	APlayerController* GetPlayerController();

	float ZoomAlpha;
	
};
