// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Constructor Called"));
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init Called"));
}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* engine = GetEngine();

	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Host executed!"));
}

void UPuzzlePlatformsGameInstance::Join(const FString& address)
{
	UEngine* engine = GetEngine();

	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Joining %s"), *address));
}