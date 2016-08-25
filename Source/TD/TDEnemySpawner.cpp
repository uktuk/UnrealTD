// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include "TDEnemy.h"
#include "TDEnemySpawner.h"


// Sets default values
ATDEnemySpawner::ATDEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = staticMesh;


}

// Called when the game starts or when spawned
void ATDEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDEnemySpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	int numDataSets = SpawnData.Num();
	if (numDataSets > 0)
	{		
		for (int i = 0; i < numDataSets; ++i)
		{
			FSpawnInfo* spawnData = &SpawnData[i];
			if ((spawnData->numToSpawn > 0) && (spawnData->enemyType != NULL))
			{
				spawnData->timeSinceLastSpawn += DeltaTime;

				if ( (spawnData->timeSinceLastSpawn >= spawnData->spawnInterval) &&
					 (spawnData->numSpawned < spawnData->numToSpawn) )
				{
					UE_LOG(LogTemp, Warning, TEXT("Spawning an enemy"));
					spawnData->timeSinceLastSpawn = 0.0f;

					// Create an enemy and spawn it
					FActorSpawnParameters spawnParameters;
					spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					ATDEnemy* newEnemy = GetWorld()->SpawnActor<ATDEnemy>(spawnData->enemyType, spawnParameters);
					newEnemy->SetActorLocation(this->GetActorLocation());
					newEnemy->SetWaypoints(spawnData->waypointRoute);
					//TODO:: pass in the waypoint marker information

					spawnData->numSpawned++;

				}
			}	
		}
	}
}

