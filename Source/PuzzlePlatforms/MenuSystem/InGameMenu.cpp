// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"

void UInGameMenu::Setup()
{
	AddToViewport(0);

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();

	if (!ensure(playerController != nullptr)) return;

	FInputModeGameAndUI inputMode;

	inputMode.SetWidgetToFocus(TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = true;
}

void UInGameMenu::SetMenuInterface(IMenuInterface* mi)
{
	menuInterface = mi;
}

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (!ensure(cancelButton != nullptr)) return false;
	if (!ensure(mainMenuButton != nullptr)) return false;

	cancelButton->OnClicked.AddDynamic(this, &UInGameMenu::Disable);
	mainMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::GoToMainMenu);

	return true;
}

void UInGameMenu::GoToMainMenu()
{
	menuInterface->GoToMainMenu();
}

void UInGameMenu::Disable()
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