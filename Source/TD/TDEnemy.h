// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TDEnemy.generated.h"


UCLASS()
class TD_API ATDEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Take damage from another source 
	 *	Meant to be called from the damage dealing source ex. Projectile fired from tower
	 *	@param damageAmount -> the amount of damage to take
	 *	@param sender       -> the Actor which send the damage source ex. which tower/structure
	 */
	void TakeDamage(float damageAmount, AActor* sender);

	void SetWaypoints(TArray<class ATDWaypoint*> waypoints);

protected:
	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Skeletal mesh for the Enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* Mesh;

	// CapsuleComponent for collision detection
	UPROPERTY(EditAnywhere, Category = "Collsion")
	UCapsuleComponent* CollisionCapsule;

	UPROPERTY(EditAnywhere, Category = "Defaults")
	float moveSpeed;

	// Maximum health
	UPROPERTY(visibleAnywhere, BlueprintReadonly, Category = "Defaults")
	float maxHealth;
	
	// Current health (out of maximum health)
	UPROPERTY(visibleAnywhere, BlueprintReadonly, Category = "Defaults")
	float currentHealth;	

	UPROPERTY()
	uint16 nextWaypoint;

	UPROPERTY()
	TArray<class ATDWaypoint*> waypointRoute;
};