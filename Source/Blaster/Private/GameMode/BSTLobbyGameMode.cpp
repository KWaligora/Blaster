// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BSTLobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

ABSTLobbyGameMode::ABSTLobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ABSTLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
		}
	}
}
