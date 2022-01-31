// Project Revival. All Rights Reserved


#include "UI/PauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Menu/OptionsWidget.h"

void UPauseWidget::NativeOnInitialized()
{
	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClearPause);
	}

	if (SaveGameButton)
	{
		SaveGameButton->OnClicked.AddDynamic(this, &UPauseWidget::OnSaveGame);
	}
	
	if (LoadGameButton)
	{
		LoadGameButton->OnClicked.AddDynamic(this, &UPauseWidget::OnLoadGame);
	}

	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UPauseWidget::OnOptions);
	}
	
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UPauseWidget::OnQuitGame);
	}
}

void UPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	GetWorld()->GetAuthGameMode()->ClearPause();
}

void UPauseWidget::OnOptions()
{
	if (OptionsWidgetClass)
	{
		// RemoveFromParent();
		UOptionsWidget* OptionsWidget = CreateWidget<UOptionsWidget>(GetWorld(), OptionsWidgetClass);
		OptionsWidget->AddToViewport();
	}
}

void UPauseWidget::OnSaveGame()
{
}

void UPauseWidget::OnLoadGame()
{
}

void UPauseWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
