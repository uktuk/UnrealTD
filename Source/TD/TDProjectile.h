// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TDProjectile.generated.h"

UCLASS()
class TD_API ATDProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDProjectile();
	ATDProjectile(AActor* targetActor, FVector spawnLocation);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Sets the target Enemy
	FORCEINLINE void SetTargetActor(AActor* inTargetActor) { targetActor = inTargetActor; }
	
protected:
	// Function to call when overlap begins with sphereComponent
	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// SphereComponent for collision detection
	UPROPERTY(EditAnywhere, Category = "Collision")
	USphereComponent* sphereComponent;

	// The static mesh for the projectile (may change to a particle effect)
	UPROPERTY(EditAnywhere, Category = "Visual")
	UStaticMeshComponent* staticMesh;

	// The current Actor set as the projectiles' target
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Target")
	AActor* targetActor;

	// How much damage the projectile does
	UPROPERTY(EditAnywhere, Category = "Stats")
	float damage;

	// How fast the projectile moves
	UPROPERTY(EditAnywhere, Category = "Stats")
	float moveSpeed;

	// How long until this projectile expires and destroys itself (this variable will count down until <= 0.0f)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float timeToExpire;
};
