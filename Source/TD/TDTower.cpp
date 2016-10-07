// Fill out your copyright notice in the Description page of Project Settings.

#include "TD.h"
#include "TDTower.h"


// Sets default values
ATDTower::ATDTower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	RootComponent = Mesh;

	Portrait = CreateDefaultSubobject<UTexture2D>(TEXT("Portrait"));

	TargetAreaCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Target Area Capsule"));
	TargetAreaCapsule->SetCollisionProfileName("OverlapAll");
	TargetAreaCapsule->bGenerateOverlapEvents = true;
	TargetAreaCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATDTower::OnOverlapBegin);
	TargetAreaCapsule->OnComponentEndOverlap.AddDynamic(this, &ATDTower::OnOverlapEnd);
	TargetAreaCapsule->SetCapsuleRadius(100.0f);
	TargetAreaCapsule->SetCapsuleHalfHeight(25.0f);
	TargetAreaCapsule->AttachTo(RootComponent);

	ClickCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision Component"));
	ClickCollisionSphere->SetCollisionProfileName("Pawn");
	ClickCollisionSphere->SetSphereRadius(70.0f);
	ClickCollisionSphere->AttachTo(RootComponent);

	currentTarget = nullptr;
	bHasBeenPlaced = false;
	numKills = 0;
	fireRate = 0.5f;
	timeSinceLastFire = 0.0f;
}

// Called when the game starts or when spawned
void ATDTower::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATDTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timeSinceLastFire += DeltaTime;

	// If we have an enemy in our attack area and we're able to fire
	if (targetsList.Num() != 0)
	{
		if (currentTarget == nullptr)
		{
			currentTarget = targetsList[0];
		}

		if ((timeSinceLastFire >= fireRate) && projectileType)
		{
			// Fire a projectile
			FActorSpawnParameters spawnInfo;
			spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnInfo.Owner = this;

			ATDProjectile* spawnedProjectile = GetWorld()->SpawnActor<ATDProjectile>(projectileType, spawnInfo);
			spawnedProjectile->SetActorLocation(GetActorLocation()); // Set to barrel socket
			spawnedProjectile->SetTargetActor(currentTarget);

			timeSinceLastFire = 0.0f;
		}
	}
}

void ATDTower::OnSelected()
{
	Mesh->SetRenderCustomDepth(true);
}

void ATDTower::OnDeselected()
{
	Mesh->SetRenderCustomDepth(false);
}

// Called when an object overlaps with the tower's TargetAreaCapsule Component
void ATDTower::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Check if we overlapped with an enemy
		UE_LOG(LogTD, Log, TEXT("%s: Began overlap with %s"), *this->GetName(), *OtherActor->GetName());
		ATDEnemy* OtherEnemy = Cast<ATDEnemy>(OtherActor);
		if (OtherEnemy)
		{
			// If the overlapped Actor is an enemy, add it to the targets list
			targetsList.Add(OtherEnemy);
		}
	}
}

void ATDTower::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Something ended overlap
		UE_LOG(LogTD, Log, TEXT("%s: Ended overlap with %s"), *this->GetName(), *OtherActor->GetName());

		ATDEnemy* OtherEnemy = Cast<ATDEnemy>(OtherActor);
		if (OtherEnemy && (targetsList.Num() != 0))
		{
			// If the overlapped Actor is an enemy, remove it from the targets list
			targetsList.Remove(OtherEnemy);
		}
	}
}