// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include "TDProjectile.h"
#include "TDEnemy.h"


// Sets default values
ATDProjectile::ATDProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	RootComponent = staticMesh;

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	sphereComponent->SetCollisionProfileName("OverlapAll");
	sphereComponent->bGenerateOverlapEvents = true;
	sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATDProjectile::OnOverlapBegin);
	sphereComponent->AttachTo(RootComponent);

	targetActor = nullptr;
	damage = 15.0f;
	moveSpeed = 50.0f;
	timeToExpire = 10.0f;
}

ATDProjectile::ATDProjectile(AActor* inTargetActor, FVector inSpawnLocation)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	RootComponent = staticMesh;

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	sphereComponent->AttachTo(RootComponent);

	targetActor = inTargetActor;
	damage = 15.0f;
	moveSpeed = 50.0f;
	timeToExpire = 10.0f;

	SetActorLocation(inSpawnLocation);
}

// Called when the game starts or when spawned
void ATDProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	timeToExpire -= DeltaTime;

	// If the time this object is allowed to live has reached 0, Destroy itself
	if (timeToExpire <= 0.0f)
	{
		Destroy();
	}
	else if (targetActor)
	{
		// Find direction to our target and head towards it
		FVector location = GetActorLocation();
		FVector destination = targetActor->GetActorLocation() - location;
		destination.Normalize();

		SetActorLocation(location + (destination * moveSpeed * DeltaTime));
	}
}

void ATDProjectile::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && (OtherActor != GetOwner()) && OtherComp)
	{
		ATDEnemy* OtherEnemy = Cast<ATDEnemy>(OtherActor);
		if (OtherEnemy)
		{
			OtherEnemy->TakeDamage(damage, GetOwner());
			Destroy();
		}
	}
}