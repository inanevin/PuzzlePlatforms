// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerListElement.h"
#include "Components/TextBlock.h"
#include "MainMenu.h"
#include "Components/Button.h"


void UServerListElement::Setup(UMainMenu* parent, uint32 index, const FText& name)
{
	m_Index = index;

	serverName->SetText(name);

	if (serverElementButton != nullptr)
		serverElementButton->OnClicked.AddDynamic(this, &UServerListElement::ElementClicked);

	parentMenu = parent;
}

void UServerListElement::ElementClicked()
{
	if (parentMenu != nullptr)
		parentMenu->SetSelectedIndex(m_Index);
}