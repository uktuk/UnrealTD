// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TDEnemy.h"
#include "TDProjectile.h"
#include "TDTower.generated.h"

UCLASS()
class TD_API ATDTower : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATDTower();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Increments the kill count by +1	
	FORCEINLINE void IncrementKills() { numKills += 1; }

	UFUNCTION()
		void OnSelected();

	UFUNCTION()
		void OnDeselected();

protected:
	// Function to call when overlap begins with TargetAreaCapsule
	UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function to call when overlap ends with TargetAreaCapsule
	UFUNCTION()
		void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// bool FORCEINLINE GetHasBeenPlaced() { return bHasBeenPlaced; };
	// Has this tower been placed on a tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		bool bHasBeenPlaced;

protected:
	// Skeletal mesh for the Tower
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		USkeletalMeshComponent* Mesh;

	// This Unit's Portrait for UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		UTexture2D* Portrait;

	// The Capsule area to detect enemies (firing range/area)
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* TargetAreaCapsule;

	// The Sphere collision component for selecting the tower
	UPROPERTY(EditAnywhere)
		USphereComponent* ClickCollisionSphere;

	// Currently targeted Enemy
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target")
		AActor* currentTarget;

	// List of Enemies that enter its firing range (list of targets to shoot at)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target")
		TArray<ATDEnemy*> targetsList;

	// class of the type of ammo/projectile this tower will use
	UPROPERTY(EditAnywhere, Category = "Ammo")
		TSubclassOf<ATDProjectile> projectileType;

	// Number of kills this tower has achieved
	UPROPERTY(VisibleAnywhere, Category = "Stats")
		int32 numKills;

	// How often the tower will fire
	UPROPERTY(EditAnywhere, Category = "Stats")
		float fireRate;

	// How much time has passed since the last time the tower fired
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
		float timeSinceLastFire;
};
