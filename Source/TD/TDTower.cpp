// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include "TDTower.h"


// Sets default values
ATDTower::ATDTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	Mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ATDTower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDTower::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
