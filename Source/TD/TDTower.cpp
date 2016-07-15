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

	TargetAreaCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Target Area Capsule"));
	TargetAreaCapsule->SetCollisionProfileName("OverlapAll");
	TargetAreaCapsule->bGenerateOverlapEvents = true;
	TargetAreaCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATDTower::OnOverlapBegin);
	TargetAreaCapsule->SetCapsuleRadius(100.0f);
	TargetAreaCapsule->SetCapsuleHalfHeight(25.0f);
	TargetAreaCapsule->AttachTo(RootComponent);

	bHasBeenPlaced = false;
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

// Called when an object overlaps with the tower's TargetAreaCapsule Component
void ATDTower::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Check if we overlapped with an enemy
		UE_LOG(LogTD, Log, TEXT("%s: Overlapped with %s"), *this->GetName(), *OtherActor->GetName());
	}
}