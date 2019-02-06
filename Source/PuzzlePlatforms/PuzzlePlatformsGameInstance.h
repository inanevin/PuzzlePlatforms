// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSessionInterface.h"
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


		

	virtual void Host(const FString& serverName) override;
	virtual void Join(uint32 index) override;
	virtual void GoToMainMenu() override;
	virtual void SearchForSessions() override;
	virtual bool CancelFindSessions() override;
	void StartSession();

protected:

	virtual void Init() override;

private:


	TSubclassOf<class UUserWidget> menuClass;
	TSubclassOf<class UUserWidget> inGameMenuClass;
	class UInGameMenu* inGameMenu;
	class UMainMenu* mainMenu;
	


	IOnlineSessionPtr sessionInterface;
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;

	void OnCreateSessionComplete(FName sessionName, bool success);
	void OnDestroySessionComplete(FName sessionName, bool success);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
	void OnFindSessionsComplete(bool success);
	void CreateSession();

	uint32 lastSessionSearchResult;
	FString desiredServerName;
};
