#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BSTHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UTexture2D* CrosshairCenter;

	UPROPERTY(EditAnywhere)
	class UTexture2D* CrosshairLeft;

	UPROPERTY(EditAnywhere)
	class UTexture2D* CrosshairRight;

	UPROPERTY(EditAnywhere)
	class UTexture2D* CrosshairTop;

	UPROPERTY(EditAnywhere)
	class UTexture2D* CrosshairBottom;

	float CrosshairSpreed;

	FLinearColor CrosshairColor;
};

/**
 * 
 */
UCLASS()
class BLASTER_API ABSTHUD : public AHUD
{
	GENERATED_BODY()

public:
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere, Category="Player Stats")
	TSubclassOf<UUserWidget> CharacterOverlayClass;
	
	UPROPERTY()
	class UBSTCharacterOverlay* CharacterOverlay;
	
	virtual void DrawHUD() override;

private:
	UPROPERTY(EditAnywhere)
	float CrosshairSpreedMax = 16.0f;

	virtual void BeginPlay() override;
	void AddCharacterOverlay();
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spreed, FLinearColor Color);
};
