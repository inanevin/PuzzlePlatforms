// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Constructor Called"));

	ConstructorHelpers::FClassFinder<APlatformTrigger> PlatformTrigger(TEXT("/Game/PuzzlePlatforms/BP_PlatformTrigger"));
	ConstructorHelpers::FClassFinder<UUserWidget> MenuWidget(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidget(TEXT("/Game/MenuSystem/WBP_InGameMenu"));

	if (!ensure(PlatformTrigger.Class != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *PlatformTrigger.Class->GetName());

	menuClass = MenuWidget.Class;
	inGameMenuClass = InGameMenuWidget.Class;


}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init Called"));
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *menuClass->GetName());

	if (!ensure(inGameMenuClass != nullptr)) return;
	inGameMenu = CreateWidget<UInGameMenu>(this, inGameMenuClass);
	inGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(menuClass != nullptr)) return;

	mainMenu = CreateWidget<UMainMenu>(this, menuClass);

	if (!ensure(mainMenu != nullptr)) return;

	mainMenu->Setup();
	mainMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (!ensure(inGameMenu != nullptr)) return;

	if (!ensure(inGameMenu != nullptr)) return;

	inGameMenu = CreateWidget<UInGameMenu>(this, inGameMenuClass);
	inGameMenu->ActivateMenu();
	inGameMenu->SetMenuInterface(this);

	//inGameMenuOpen = true;

	/*if (!inGameMenuOpen)
	{
		inGameMenu = CreateWidget<UInGameMenu>(this, inGameMenuClass);

		
	}
	else
	{
		inGameMenu->DeactivateMenu();
		inGameMenuOpen = false;
	}
	*/
}

void UPuzzlePlatformsGameInstance::Host()
{
	UE_LOG(LogTemp, Warning, TEXT("Host Called"));

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

void UPuzzlePlatformsGameInstance::GoToMainMenu()
{

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}