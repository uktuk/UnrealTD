// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include "TDEnemy.h"
#include "TDTower.h"


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

	maxHealth = 100.0f;
	currentHealth = 100.0f;
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