#include "BSTGameMode.h"

#include "Characters/BSTCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ABSTGameMode::PlayerEliminated(ABSTCharacter* ElimmedCharacter, ABSTPlayerController* VictimController,
                                    ABSTPlayerController* AttackerController)
{
	if (ElimmedCharacter != nullptr)
	{
		ElimmedCharacter->Eliminated();
	}	
}

void ABSTGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimController)
{
	if (ElimmedCharacter != nullptr)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimController != nullptr)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimController, PlayerStarts[Selection]);
	}
}
