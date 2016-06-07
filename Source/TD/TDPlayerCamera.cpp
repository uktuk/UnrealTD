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