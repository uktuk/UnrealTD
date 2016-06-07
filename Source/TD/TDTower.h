// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
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
	virtual void Tick( float DeltaSeconds ) override;

public:

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;
};
