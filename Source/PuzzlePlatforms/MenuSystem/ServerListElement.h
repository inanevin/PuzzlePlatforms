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

	void Setup(class UMainMenu* parent, uint32 index, const FText& serverName, const FText& hostName, uint16 currentPlayers, uint16 maxPlayers);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* serverName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* hostName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* playerCounter;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* serverElementButton;

	UPROPERTY(BlueprintReadOnly)
		bool isSelected;

private:



	UFUNCTION()
		void ElementClicked();
	
	uint32 m_Index;
	UMainMenu* parentMenu;
};
