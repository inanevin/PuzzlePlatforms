// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerListElement.h"
#include "Components/TextBlock.h"
#include "MainMenu.h"
#include "Components/Button.h"


void UServerListElement::Setup(class UMainMenu* parent, uint32 index, const FText& svName, const FText&hn, uint16 currentPlayers, uint16 maxPlayers)
{
	m_Index = index;

	serverName->SetText(svName);
	hostName->SetText(hn);
	playerCounter->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), currentPlayers, maxPlayers)));

	if (serverElementButton != nullptr)
		serverElementButton->OnClicked.AddDynamic(this, &UServerListElement::ElementClicked);

	parentMenu = parent;
}

void UServerListElement::ElementClicked()
{
	if (parentMenu != nullptr)
		parentMenu->SetSelectedIndex(m_Index);
}