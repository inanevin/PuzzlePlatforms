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
}

void AMovingPlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (HasAuthority())
	{
		FVector newLocation = GetActorLocation();
		//float deltaMovement = (FMath::Sin((runningTime + deltaTime) * movementSpeed) - FMath::Sin(runningTime * movementSpeed));
		newLocation.X += deltaTime * movementAmount;
		runningTime += deltaTime;
		SetActorLocation(newLocation);
	}

}