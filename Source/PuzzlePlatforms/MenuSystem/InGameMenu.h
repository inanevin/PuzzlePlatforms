// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MenuInterface.h"
#include "InGameMenu.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetMenuInterface(IMenuInterface* mi);
	void Setup();
	void Disable();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* cancelButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* mainMenuButton;

protected:

	virtual bool Initialize() override;

private:

	IMenuInterface* menuInterface;
	void GoToMainMenu();
};
