// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"

bool UMainMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	
	if (!ensure(hostButton != nullptr)) return false;

	hostButton->OnClicked.AddDynamic(this, &UMainMenu::HostButtonClicked);
	joinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinButtonClicked);
	joinIPButton->OnClicked.AddDynamic(this, &UMainMenu::JoinIPButtonClicked);
	backButton->OnClicked.AddDynamic(this, &UMainMenu::BackButtonClicked);
	exitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitButtonClicked);
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
;	if (!ensure(menuInterface != nullptr)) return;
	menuInterface->Host();
}

void UMainMenu::JoinButtonClicked()
{
	if (!ensure(menuInterface != nullptr)) return;
	switcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::JoinIPButtonClicked()
{
	FText addr = ipText->GetText();
	menuInterface->Join(addr.ToString());
}

void UMainMenu::BackButtonClicked()
{
	switcher->SetActiveWidget(MainMenu);
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

void UMainMenu::SetMenuInterface(IMenuInterface* mi)
{
	menuInterface = mi;
}