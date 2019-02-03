// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	
	if (!ensure(hostButton != nullptr)) return false;

	hostButton->OnClicked.AddDynamic(this, &UMainMenu::HostButtonClicked);
	joinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinButtonClicked);

	return true;
}


void UMainMenu::HostButtonClicked()
{
	if (!ensure(menuInterface != nullptr)) return;
	menuInterface->Host();
}

void UMainMenu::JoinButtonClicked()
{
	if (!ensure(menuInterface != nullptr)) return;
	menuInterface->Join(TEXT("192.168.1.33"));
}