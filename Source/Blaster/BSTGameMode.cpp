#include "BSTGameMode.h"

#include "Characters/BSTCharacter.h"

void ABSTGameMode::PlayerEliminated(ABSTCharacter* ElimmedCharacter, ABSTPlayerController* VictimController,
                                    ABSTPlayerController* AttackerController)
{
	if (ElimmedCharacter != nullptr)
	{
		ElimmedCharacter->Eliminated();
	}	
}
