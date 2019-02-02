// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	if (HasAuthority())
		SetMobility(EComponentMobility::Movable);

}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	globalStartLocation = GetActorLocation();
	globalTargetLocation = GetTransform().TransformPosition(locationVector);
}

void AMovingPlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (HasAuthority())
	{
		FVector newLocation = GetActorLocation();
		FVector diffVector = (globalTargetLocation - globalStartLocation).GetSafeNormal();
		//float deltaMovement = (FMath::Sin((runningTime + deltaTime) * movementSpeed) - FMath::Sin(runningTime * movementSpeed));
		newLocation += deltaTime * diffVector * movementSpeed;
		//runningTime += deltaTime;
		SetActorLocation(newLocation);
	}

}