// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include "TDPlayerCamera.h"


UTDPlayerCamera::UTDPlayerCamera(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZoomAlpha = 1.0f;
	CameraScrollSpeed = 4000.0f;
	MinZoomLevel = 0.4f;
	MaxZoomLevel = 1.0f;
}

void UTDPlayerCamera::OnZoomIn()
{
	SetZoomLevel(ZoomAlpha - 0.1f);
}

void UTDPlayerCamera::OnZoomOut()
{
	SetZoomLevel(ZoomAlpha + 0.1f);
}

void UTDPlayerCamera::UpdateCameraMovement()
{

}

void UTDPlayerCamera::MoveForward(float val)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn != NULL)
	{
		APlayerController* Controller = GetPlayerController();
		if ((val != 0.0f) && (Controller != NULL))
		{
			const FRotationMatrix R(Controller->PlayerCameraManager->GetCameraRotation());
			const FVector WorldSpaceAccel = R.GetScaledAxis(EAxis::X) * 100.0f;

			// Transform to world space and add it
			OwnerPawn->AddMovementInput(WorldSpaceAccel, val);
		}
	}
}

void UTDPlayerCamera::MoveRight(float val)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn != NULL)
	{
		APlayerController* Controller = GetPlayerController();
		if ((val != 0.0f) && (Controller != NULL))
		{
			const FRotationMatrix R(Controller->PlayerCameraManager->GetCameraRotation());
			const FVector WorldSpaceAccel = R.GetScaledAxis(EAxis::Y) * 100.0f;

			// Transform to world space and add it
			OwnerPawn->AddMovementInput(WorldSpaceAccel, val);
		}
	}
}

void UTDPlayerCamera::SetZoomLevel(float newLevel)
{
	ZoomAlpha = FMath::Clamp(newLevel, MinZoomLevel, MaxZoomLevel);
}

APawn* UTDPlayerCamera::GetOwnerPawn()
{
	return Cast<APawn>(GetOwner());
}

APlayerController* UTDPlayerCamera::GetPlayerController()
{
	APlayerController* Controller = NULL;
	APawn* Owner = GetOwnerPawn();
	if (Owner != NULL)
	{
		Controller = Cast<APlayerController>(Owner->GetController());
	}
	return Controller;
}