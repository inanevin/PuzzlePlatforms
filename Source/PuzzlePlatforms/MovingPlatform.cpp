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

void AMovingPlatform::AddActiveTrigger()
{
	activeTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (activeTriggers > 0) 	activeTriggers--;
}

void AMovingPlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (activeTriggers > 0)
	{

		if (HasAuthority())
		{
			/*FVector actorLocation = GetActorLocation();
			FVector moveVector = vectorSwap == false ? (globalTargetLocation - globalStartLocation).GetSafeNormal() : (globalStartLocation - globalTargetLocation).GetSafeNormal();
			FVector conditionalDifference = vectorSwap == false ? (globalTargetLocation - actorLocation).GetSafeNormal() : (globalStartLocation - actorLocation).GetSafeNormal();

			if (FVector::DotProduct(moveVector, conditionalDifference) < 0)
			{
				vectorSwap = !vectorSwap;
			}

			//float deltaMovement = (FMath::Sin((runningTime + deltaTime) * movementSpeed) - FMath::Sin(runningTime * movementSpeed));
			actorLocation += deltaTime * moveVector * movementSpeed;
			//runningTime += deltaTime;
			SetActorLocation(actorLocation);*/


			FVector location = GetActorLocation();
			float journeyLength = (globalTargetLocation - globalStartLocation).Size();
			float journeyTravelled = (location - globalStartLocation).Size();

			if (journeyTravelled >= journeyLength)
			{
				FVector temp = globalStartLocation;
				globalStartLocation = globalTargetLocation;
				globalTargetLocation = temp;
			}

			FVector dir = (globalTargetLocation - globalStartLocation).GetSafeNormal();
			location += deltaTime * movementSpeed * dir;
			SetActorLocation(location);
		}
	}


}