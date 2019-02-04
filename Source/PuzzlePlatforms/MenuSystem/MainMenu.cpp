// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "ServerListElement.h"
#include "ConstructorHelpers.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerListElementWidget(TEXT("/Game/MenuSystem/WBP_ServerListElement"));

	if (!ensure(ServerListElementWidget.Class != nullptr)) return;

	serverListElementClass = ServerListElementWidget.Class;

}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	
	if (!ensure(hostButton != nullptr)) return false;

	hostButton->OnClicked.AddDynamic(this, &UMainMenu::HostButtonClicked);
	joinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinButtonClicked);
	joinIPButton->OnClicked.AddDynamic(this, &UMainMenu::JoinIPButtonClicked);
	backButton->OnClicked.AddDynamic(this, &UMainMenu::BackButtonClicked);
	exitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitButtonClicked);
	refreshServerListButton->OnClicked.AddDynamic(this, &UMainMenu::RefreshServerListButtonClicked);

	return true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel * inLevel, UWorld * inWorld)
{
	RemoveFromViewport();

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();

	if (!ensure(playerController != nullptr)) return;

	FInputModeGameOnly inputMode;

	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = false;
}


void UMainMenu::HostButtonClicked()
{
	if (!ensure(menuInterface != nullptr)) return;
	menuInterface->Host();
}

void UMainMenu::JoinButtonClicked()
{
	if (!ensure(menuInterface != nullptr)) return;
	switcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::JoinIPButtonClicked()
{
	//FText addr = ipText->GetText();
	//menuInterface->Join(addr.ToString());
}

void UMainMenu::BackButtonClicked()
{
	switcher->SetActiveWidget(MainMenu);
}

void UMainMenu::RefreshServerListButtonClicked()
{
	if (!ensure(menuInterface != nullptr)) return;
	menuInterface->SearchForSessions();
}

void UMainMenu::ExitButtonClicked()
{
	UWorld* world = GetWorld();

	if (!ensure(world != nullptr))return;

	APlayerController* playerController = world->GetFirstPlayerController();

	if (!ensure(playerController != nullptr)) return;

	playerController->ConsoleCommand(TEXT("quit"), true);
}



void UMainMenu::Setup()
{
	AddToViewport(0);

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();

	if (!ensure(playerController != nullptr)) return;

	FInputModeUIOnly inputMode;

	inputMode.SetWidgetToFocus(TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = true;
}

void UMainMenu::PopulateServerList(const TArray<FString>& sessionIDs)
{
	if (serverListElementClass == nullptr) return;

	serverList->ClearChildren();

	for (int i = 0; i < sessionIDs.Num(); i++)
	{
		UServerListElement* widget = CreateWidget<UServerListElement>(this, serverListElementClass);
		widget->SetServerName(FText::FromString(sessionIDs[i]));
		serverList->AddChild(widget);
	}
	
}

void UMainMenu::SetMenuInterface(IMenuInterface* mi)
{
	menuInterface = mi;
}