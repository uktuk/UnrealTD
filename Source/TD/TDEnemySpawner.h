// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TDWaypoint.h"
#include "TDEnemySpawner.generated.h"

USTRUCT()
struct FSpawnInfo
{
	GENERATED_BODY()

public:
	// The Enemy type (class) to spawn 
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<class ATDEnemy> enemyType;

	// The list of waypoints the spawned enemy will take
	UPROPERTY(EditAnywhere, Category = "Default")
	TArray<ATDWaypoint*> waypointRoute;

	// Number of enemies of this type to spawn
	UPROPERTY(EditAnywhere, Category = "Default")
	uint16 numToSpawn = 0;

	// Position in which order to spawn the enemies
	// Multiple spawns in the same order will spawn simultaniously
	UPROPERTY(EditAnywhere, Category = "Default")
	uint16 spawnOrder = 0;

	// Spawn interval (how often in seconds) to spawn the enemy at
	UPROPERTY(EditAnywhere, Category = "Default")
	float spawnInterval = 0.0f;

	// time elapsed since the last enemy was spawned from this SpawnInfo section
	UPROPERTY()
	float timeSinceLastSpawn = 0.0f;

	// How many enemies have been spawned by this SpawnInfo section
	UPROPERTY()
	uint16 numSpawned = 0;
};



UCLASS()
class TD_API ATDEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDEnemySpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
private:	
	UPROPERTY(EditAnywhere, Category = "Default")
	TArray<FSpawnInfo> SpawnData;

	UPROPERTY(EditAnywhere, Category = "Default")
	UStaticMeshComponent* staticMesh;
};
