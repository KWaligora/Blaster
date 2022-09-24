#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BSTPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABSTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetHUDHealth(float Health, float MaxHealth);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ABSTHUD* BSTHUD;
};
