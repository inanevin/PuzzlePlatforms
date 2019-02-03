// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Constructor Called"));

	ConstructorHelpers::FClassFinder<APlatformTrigger> PlatformTrigger(TEXT("/Game/PuzzlePlatforms/BP_PlatformTrigger"));
	ConstructorHelpers::FClassFinder<UUserWidget> MenuWidget(TEXT("/Game/MenuSystem/WBP_MainMenu"));

	if (!ensure(PlatformTrigger.Class != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *PlatformTrigger.Class->GetName());

	menuClass = MenuWidget.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init Called"));
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *menuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(menuClass != nullptr)) return;
	UUserWidget* menu = CreateWidget<UUserWidget>(this, menuClass);
	if (!ensure(menu != nullptr)) return;
	menu->AddToViewport(0);
}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* engine = GetEngine();

	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Host executed!"));

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& address)
{
	UEngine* engine = GetEngine();

	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Joining %s"), *address));

	APlayerController* playerController = GetFirstLocalPlayerController();

	if (!ensure(engine != nullptr)) return;

	playerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);
	
}