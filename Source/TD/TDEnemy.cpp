// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include "TDEnemy.h"
#include "TDTower.h"
#include "TDWaypoint.h"


// Sets default values
ATDEnemy::ATDEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	RootComponent = Mesh;
	//Mesh->AttachTo(RootComponent);

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));
	CollisionCapsule->SetCollisionProfileName("OverlapAll");
	CollisionCapsule->bGenerateOverlapEvents = true;
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATDEnemy::OnOverlapBegin);
	CollisionCapsule->SetCapsuleRadius(100.0f);
	CollisionCapsule->SetCapsuleHalfHeight(25.0f);
	CollisionCapsule->AttachTo(RootComponent);

	moveSpeed = 5.0f;
	maxHealth = 100.0f;
	currentHealth = 100.0f;
	nextWaypoint = 0;
}

// Called when the game starts or when spawned
void ATDEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FVector actorLocation = GetActorLocation();

	if (waypointRoute.Num() > 0)
	{
		// If we've arrived at the next waypoint and it's not the final waypoint
		if ( (FVector::DistSquared(actorLocation, waypointRoute[nextWaypoint]->GetActorLocation()) < 15.0f) &&
			 ((nextWaypoint + 1) != waypointRoute.Num()) )
		{
			++nextWaypoint;
		}
		else // Move towards next waypoint
		{
			FVector toNextWaypoint = waypointRoute[nextWaypoint]->GetActorLocation() - actorLocation;
			toNextWaypoint.Normalize();

			SetActorLocation(actorLocation + (toNextWaypoint * moveSpeed * DeltaTime));
		}
	}

}

/** Take damage from another source
  *	Meant to be called from the damage dealing source ex. Projectile fired from tower
  *	@param damageAmount -> the amount of damage to take
  *	@param sender       -> the Actor which send the damage source ex. which tower/structure
  */
void ATDEnemy::TakeDamage(float damageAmount, AActor* sender)
{
	if (sender)
	{
		currentHealth -= damageAmount;
		
		ATDTower* senderTower = Cast<ATDTower>(sender);
		if (currentHealth <= 0.0f && senderTower)
		{
			senderTower->IncrementKills();

			// Flag for destruction/death animation/effect
			Destroy();
		}
	}
}

// Called when an object overlaps with the Enemys' CollisionCapsule component
void ATDEnemy::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// overlapped with something		
		UE_LOG(LogTD, Log, TEXT("%s: Overlapped with %s"), *this->GetName(), *OtherActor->GetName());
	}
}


void ATDEnemy::SetWaypoints(TArray<ATDWaypoint*> waypoints)
{
	waypointRoute = waypoints;
}