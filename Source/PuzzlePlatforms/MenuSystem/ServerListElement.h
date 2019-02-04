// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerListElement.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerListElement : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetServerName(const FText& serverName);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* serverName;

};