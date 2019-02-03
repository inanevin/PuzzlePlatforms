// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"

void UInGameMenu::ActivateMenu()
{
	AddToViewport(0);

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();

	if (!ensure(playerController != nullptr)) return;

	FInputModeUIOnly inputMode;

	inputMode.SetWidgetToFocus(this->TakeWidget());
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
	UE_LOG(LogTemp, Warning, TEXT("Initialized!!!!!"));

	cancelButton->OnClicked.AddDynamic(this, &UInGameMenu::DeactivateMenu);
	mainMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::GoToMainMenu);

	return true;
}

void UInGameMenu::GoToMainMenu()
{
	DeactivateMenu();
	if (!ensure(menuInterface != nullptr))return;
	menuInterface->GoToMainMenu();
}

void UInGameMenu::DeactivateMenu()
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