// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

}

void AMovingPlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	FVector newLocation = GetActorLocation();

	float deltaMovement = (FMath::Sin((runningTime + deltaTime) * movementSpeed) - FMath::Sin(runningTime * movementSpeed));
	newLocation.X += deltaMovement * movementAmount;
	runningTime += deltaTime;
	SetActorLocation(newLocation);

}