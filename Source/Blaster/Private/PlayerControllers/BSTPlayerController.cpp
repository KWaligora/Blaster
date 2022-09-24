#include "PlayerControllers/BSTPlayerController.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/BSTCharacterOverlay.h"
#include "HUD/BSTHUD.h"

void ABSTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BSTHUD = Cast<ABSTHUD>(GetHUD());
}

void ABSTPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	BSTHUD = BSTHUD == nullptr ? Cast<ABSTHUD>(GetHUD())  : BSTHUD;

	bool bHUDValid = BSTHUD != nullptr &&
		BSTHUD->CharacterOverlay &&
		BSTHUD->CharacterOverlay->HealthBar &&
		BSTHUD->CharacterOverlay->HealthText;
	
	if (bHUDValid)
	{
		const float HealthPercent = Health/MaxHealth;
		BSTHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BSTHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}