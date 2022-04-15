// Project Revival. All Rights Reserved


#include "UI/CrosshairWidget.h"

void UCrosshairWidget::OnHitDetected(bool IsInHead)
{
	if (!IsInHead)
	{
		CrosshairImage->SetBrushFromTexture(CrosshairHit);
		CrosshairImage->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
		DotImage->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
		PlayHitAnimation();	
	}
	else
	{
		CrosshairImage->SetBrushFromTexture(CrosshairHit);
		CrosshairImage->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
		DotImage->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
		PlayHeadHitAnimation();	
	}
}

void UCrosshairWidget::OnStopAnimation()
{
	CrosshairImage->SetBrushFromTexture(CrosshairNormal);
	CrosshairImage->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	DotImage->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	CrosshairImage->SetRenderScale(FVector2D(1, 1));
}

void UCrosshairWidget::NativeOnInitialized()
{
	CrosshairImage->SetBrushFromTexture(CrosshairNormal);
}
