// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	AMovingPlatform();

	void AddActiveTrigger();
	void RemoveActiveTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float movementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float movementAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (MakeEditWidget = true))
		FVector locationVector;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

private:

	UPROPERTY(EditAnywhere)
		int activeTriggers = 1;

	float runningTime;
	FVector globalTargetLocation;
	FVector globalStartLocation;
	bool vectorSwap = false;

};
