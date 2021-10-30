// Project Revival. All Rights Reserved


#include "Menu/LevelItemWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void ULevelItemWidget::SetLevelData(const FLevelData& NewLevelData)
{
	LevelData = NewLevelData;
	if (LevelNameTextBlock)
	{
		LevelNameTextBlock->SetText(FText::FromName(NewLevelData.LevelDisplayName));
	}
	if (LevelImage)
	{
		LevelImage->SetBrushFromTexture(NewLevelData.LevelThumb);
	}
}

void ULevelItemWidget::SetSelected(bool IsSelected)
{
	if (LevelImage)
	{
		LevelImage->SetColorAndOpacity(IsSelected ? FLinearColor::White : FLinearColor(0.25, 0.35, 0.7, 1)); 
	}
}

void ULevelItemWidget::NativeOnInitialized()
{
	if (LevelSelectButton)
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &ULevelItemWidget::OnLevelItemClicked);
		LevelSelectButton->OnHovered.AddDynamic(this, &ULevelItemWidget::OnLevelItemHovered);
		LevelSelectButton->OnUnhovered.AddDynamic(this, &ULevelItemWidget::OnLevelItemUnHovered);
	}
}

void ULevelItemWidget::OnLevelItemClicked()
{
	OnLevelSelected.Broadcast(LevelData);
}

void ULevelItemWidget::OnLevelItemHovered()
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULevelItemWidget::OnLevelItemUnHovered()
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
