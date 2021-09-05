// Project Revival. All Rights Reserved


#include "UI/PauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void UPauseWidget::NativeOnInitialized()
{
	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClearPause);
	}
}

void UPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	GetWorld()->GetAuthGameMode()->ClearPause();
}
