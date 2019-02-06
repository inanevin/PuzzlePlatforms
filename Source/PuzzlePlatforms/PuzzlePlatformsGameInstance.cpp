// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "OnlineSessionSettings.h"


const static FName SESSION_NAME = NAME_GameSession;
const static int MAX_SEARCH_RESULTS = 1000;
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");
UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{

	ConstructorHelpers::FClassFinder<UUserWidget> MenuWidget(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidget(TEXT("/Game/MenuSystem/WBP_InGameMenu"));


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
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);


			SearchForSessions();

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

	world->ServerTravel("/Game/PuzzlePlatforms/Maps/Lobby?listen");
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool success)
{
	UE_LOG(LogTemp, Warning, TEXT("Session search finished!"));

	if (!success || !sessionSearch.IsValid() || mainMenu == nullptr) return;

	TArray<FServerData> sessionIDs;

	for (int i = 0; i < sessionSearch->SearchResults.Num(); i++)
	{
		FString sessionID = sessionSearch->SearchResults[i].GetSessionIdStr();
		FServerData data;
		FString serverName;

		if (sessionSearch->SearchResults[i].Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, serverName))
			data.serverName = serverName;
		else
			data.serverName = "User {0}";

		data.hostUserName = sessionSearch->SearchResults[i].Session.OwningUserName;
		data.maxPlayers = sessionSearch->SearchResults[i].Session.SessionSettings.NumPublicConnections;
		data.currentPlayers = data.maxPlayers - sessionSearch->SearchResults[i].Session.NumOpenPublicConnections;
		sessionIDs.Add(data);
		UE_LOG(LogTemp, Warning, TEXT("Search result %d %s custom search result %s"), i, *sessionID);
	}


	mainMenu->PopulateServerList(sessionIDs);
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName sessionName, bool success)
{
	if (!success) return;
	CreateSession();
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	APlayerController* playerController = GetFirstLocalPlayerController();

	if (playerController != nullptr && sessionInterface.IsValid())
	{
		FString travelURL;
		FName portType;

		if (sessionInterface->GetResolvedConnectString(SESSION_NAME, travelURL))
		{
			if (result != EOnJoinSessionCompleteResult::Success)
				playerController->ClientTravel(travelURL, ETravelType::TRAVEL_Absolute);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("COULD NOT JOIN SESSION"));
	}



}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (!sessionInterface.IsValid()) return;

	FOnlineSessionSettings sessionSettings;

	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		sessionSettings.bIsLANMatch = true;
	}
	else
	{
		sessionSettings.bIsLANMatch = false;
	}

	sessionSettings.NumPublicConnections = 3;
	sessionSettings.bShouldAdvertise = true;
	sessionSettings.bUsesPresence = true;	// use lobies instead of steam servers.
	sessionSettings.Set(SERVER_NAME_SETTINGS_KEY, desiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
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

void UPuzzlePlatformsGameInstance::Host(const FString& serverName)
{
	desiredServerName = serverName;

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

void UPuzzlePlatformsGameInstance::Join(uint32 index)
{

	if (!sessionInterface.IsValid()) return;
	if (!sessionSearch.IsValid()) return;

	lastSessionSearchResult = index;
	sessionInterface->JoinSession(0, SESSION_NAME, sessionSearch->SearchResults[index]);

}

void UPuzzlePlatformsGameInstance::GoToMainMenu()
{

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/PuzzlePlatforms/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::SearchForSessions()
{
	sessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (sessionInterface.IsValid() && sessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Session search started!"));
		TSharedRef<FOnlineSessionSearch> sessionSearchRef = sessionSearch.ToSharedRef();

		sessionSearch->MaxSearchResults = MAX_SEARCH_RESULTS;
		sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
	}
}

bool UPuzzlePlatformsGameInstance::CancelFindSessions()
{
	return sessionInterface->CancelFindSessions();
}

void UPuzzlePlatformsGameInstance::StartSession()
{
	if (sessionInterface.IsValid())
		sessionInterface->StartSession(SESSION_NAME);
}