#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BSTGameMode.generated.h"

UCLASS()
class BLASTER_API ABSTGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PlayerEliminated(class ABSTCharacter* ElimmedCharacter, class ABSTPlayerController* VictimController, ABSTPlayerController* AttackerController);
};
