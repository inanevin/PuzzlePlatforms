// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:

	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void LoadMenu();

	UFUNCTION(BlueprintCallable)
		void LoadInGameMenu();

		

	virtual void Host() override;
	virtual void Join(const FString& address) override;
	virtual void GoToMainMenu() override;


protected:

	virtual void Init() override;

private:


	TSubclassOf<class UUserWidget> menuClass;
	TSubclassOf<class UUserWidget> inGameMenuClass;
	class UInGameMenu* inGameMenu;
	class UMainMenu* mainMenu;
	IOnlineSessionPtr sessionInterface;

	void OnCreateSessionComplete(FName sessionName, bool success);
	void OnDestroySessionComplete(FName sessionName, bool success);
	void CreateSession();
};
