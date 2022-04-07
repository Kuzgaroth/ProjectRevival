// Project Revival. All Rights Reserved


#include "Menu/DifficultyItemWidget.h"

void UDifficultyItemWidget::NativeOnInitialized()
{
	DifficultyImage->SetBrushFromTexture(IconImage);
	DifficultyNameTextBlock->SetText(FText::FromString(DifficultyName));
	DescriptionTextBlock->SetText(FText::FromString(Description));
}
