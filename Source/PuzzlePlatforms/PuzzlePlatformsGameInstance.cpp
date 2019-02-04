// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("Session Name");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{

	ConstructorHelpers::FClassFinder<APlatformTrigger> PlatformTrigger(TEXT("/Game/PuzzlePlatforms/BP_PlatformTrigger"));
	ConstructorHelpers::FClassFinder<UUserWidget> MenuWidget(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidget(TEXT("/Game/MenuSystem/WBP_InGameMenu"));

	if (!ensure(PlatformTrigger.Class != nullptr)) return;

	menuClass = MenuWidget.Class;
	inGameMenuClass = InGameMenuWidget.Class;


}

void UPuzzlePlatformsGameInstance::Init()
{
	IOnlineSubsystem* subSys = IOnlineSubsystem::Get();

	if (subSys != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Online subsystem found %s"), *subSys->GetSubsystemName().ToString());
		sessionInterface = subSys->GetSessionInterface();
		if (sessionInterface.IsValid())
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
		}
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName sessionName, bool success)
{
	if (!success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session!"));
		return;
	}

	UEngine* engine = GetEngine();

	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Host executed!"));

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName sessionName, bool success)
{
	if (!success) return;
	CreateSession();
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (!sessionInterface.IsValid()) return;

	FOnlineSessionSettings sessionSettings;
	sessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
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

	if (!ensure(inGameMenuClass != nullptr)) return;

	inGameMenu = CreateWidget<UInGameMenu>(this, inGameMenuClass);

	if (!ensure(inGameMenu != nullptr)) return;

	inGameMenu->ActivateMenu();
	inGameMenu->SetMenuInterface(this);

}

void UPuzzlePlatformsGameInstance::Host()
{
	if (sessionInterface.IsValid())
	{
		FNamedOnlineSession* existingSession = sessionInterface->GetNamedSession(SESSION_NAME);

		if (existingSession != nullptr)
		{
			sessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	
	}
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
	
	PlayerController->ClientTravel("/Game/PuzzlePlatforms/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}