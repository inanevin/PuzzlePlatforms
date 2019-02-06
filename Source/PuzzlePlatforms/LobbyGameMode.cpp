// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);

	numberOfPlayers++;

	if (numberOfPlayers >= 2)
	{
		

		GetWorldTimerManager().SetTimer(timerHandle, this, &ALobbyGameMode::PlayerTimerExpired, 5);

	}
}

void ALobbyGameMode::Logout(AController* exitingPlayer)
{
	Super::Logout(exitingPlayer);

	numberOfPlayers--;

}

void ALobbyGameMode::PlayerTimerExpired()
{
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;
	bUseSeamlessTravel = true;
	UPuzzlePlatformsGameInstance* gameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (!ensure(gameInstance != nullptr)) return;
	gameInstance->StartSession();
	world->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}