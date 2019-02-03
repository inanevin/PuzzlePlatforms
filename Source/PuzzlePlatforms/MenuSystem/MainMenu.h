// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* hostButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* joinButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* joinIPButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* backButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* exitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetSwitcher* switcher;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UEditableText* ipText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidget* JoinMenu;

	void SetMenuInterface(IMenuInterface* mi);
	void Setup();

protected:

	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* inLevel, UWorld* inWorld);

private:

	UFUNCTION()
		void HostButtonClicked();

	UFUNCTION()
		void JoinButtonClicked();

	UFUNCTION()
		void JoinIPButtonClicked();

	UFUNCTION()
		void BackButtonClicked();

	UFUNCTION()
		void ExitButtonClicked();

	IMenuInterface* menuInterface;
};
