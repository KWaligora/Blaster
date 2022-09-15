#include "HUD/BSTHUD.h"

void ABSTHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

		float SpreedScale = CrosshairSpreedMax * HUDPackage.CrosshairSpreed;

		if (HUDPackage.CrosshairCenter != nullptr)
		{
			DrawCrosshair(HUDPackage.CrosshairCenter, ViewportCenter, FVector2d(0.0f,0.0f));		
		}
		if(HUDPackage.CrosshairLeft != nullptr)
		{
			FVector2D Spreed(-SpreedScale, 0.0f);
			DrawCrosshair(HUDPackage.CrosshairLeft, ViewportCenter, Spreed);
		}
		if(HUDPackage.CrosshairRight != nullptr)
		{
			FVector2D Spreed(SpreedScale, 0.0f);
			DrawCrosshair(HUDPackage.CrosshairRight, ViewportCenter, Spreed);
		}
		if(HUDPackage.CrosshairTop != nullptr)
		{
			FVector2D Spreed(0.0f, -SpreedScale);
			DrawCrosshair(HUDPackage.CrosshairTop, ViewportCenter, Spreed);
		}
		
		if(HUDPackage.CrosshairBottom != nullptr)
		{
			FVector2D Spreed(0.0f, SpreedScale);
			DrawCrosshair(HUDPackage.CrosshairBottom, ViewportCenter, Spreed);
		}
	}
}

void ABSTHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2d Spreed)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2d TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.0f) + Spreed.X, ViewportCenter.Y - (TextureHeight / 2.0f) + Spreed.Y);

	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.0f,
		0.0f,
		1.0f,
		1.0f,
		FLinearColor::White
		);
}
