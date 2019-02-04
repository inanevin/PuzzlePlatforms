// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerListElement.h"
#include "Components/TextBlock.h"

void UServerListElement::SetServerName(const FText& name)
{
	serverName->SetText(name);
}
