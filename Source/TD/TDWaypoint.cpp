// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include "TDWaypoint.h"


// Sets default values
ATDWaypoint::ATDWaypoint()
{
	PrimaryActorTick.bCanEverTick = true;	
}

ATDWaypoint::ATDWaypoint(FVector Location)
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATDWaypoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATDWaypoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

