// Project Revival. All Rights Reserved


#include "UI/PauseWidget.h"

#include "CourtLairGameMode.h"
#include "PRGameInstance.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Menu/ConfirmationWidget.h"
#include "Menu/OptionsWidget.h"
#include "SaveSystem/PRSaveGame.h"

void UPauseWidget::NativeOnInitialized()
{
	const auto GameMode = GetWorld()->GetAuthGameMode<ACourtLairGameMode>();
	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClearPause);
		
	}

	if (CheckPointButton)
	{
		CheckPointButton->OnClicked.AddDynamic(this, &UPauseWidget::OnCheckPoint);
	}
	
	if (PlayAgainButton)
	{
		PlayAgainButton->OnClicked.AddDynamic(this, &UPauseWidget::OnPlayAgain);
	}

	if (GameMode)
	{
		CheckPointButton->SetIsEnabled(false);
		CheckPointButton->Visibility = ESlateVisibility::Collapsed;

		PlayAgainButton->SetIsEnabled(false);
		PlayAgainButton->Visibility = ESlateVisibility::Collapsed;
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
		UOptionsWidget* OptionsWidget = CreateWidget<UOptionsWidget>(GetWorld(), OptionsWidgetClass);
		OptionsWidget->AddToViewport();
	}
}

void UPauseWidget::OnCheckPoint()
{
	if (ConfirmationWidgetClass)
	{
		ConfirmationWidget = CreateWidget<UConfirmationWidget>(GetWorld(), ConfirmationWidgetClass);
		ConfirmationWidget->ConfirmButton->OnClicked.AddDynamic(this, &UPauseWidget::LoadCheckPoint);
		ConfirmationWidget->DeclineButton->OnClicked.AddDynamic(this, &UPauseWidget::CloseConfirmationWidget);
		ConfirmationWidget->SetLabelText(FText::FromString("Do you want to load the last check point?"));
		ConfirmationWidget->AddToViewport();
	}
}

void UPauseWidget::OnPlayAgain()
{
	if (ConfirmationWidgetClass)
	{
		ConfirmationWidget = CreateWidget<UConfirmationWidget>(GetWorld(), ConfirmationWidgetClass);
		ConfirmationWidget->ConfirmButton->OnClicked.AddDynamic(this, &UPauseWidget::PlayAgain);
		ConfirmationWidget->DeclineButton->OnClicked.AddDynamic(this, &UPauseWidget::CloseConfirmationWidget);
		ConfirmationWidget->SetLabelText(FText::FromString("Do you want to load the last check point?"));
		ConfirmationWidget->AddToViewport();
	}
}

void UPauseWidget::OnQuitGame()
{
	if (ConfirmationWidgetClass)
	{
		ConfirmationWidget = CreateWidget<UConfirmationWidget>(GetWorld(), ConfirmationWidgetClass);
		ConfirmationWidget->ConfirmButton->OnClicked.AddDynamic(this, &UPauseWidget::QuitGame);
		ConfirmationWidget->DeclineButton->OnClicked.AddDynamic(this, &UPauseWidget::CloseConfirmationWidget);
		ConfirmationWidget->SetLabelText(FText::FromString("Are you sure you want to exit?"));
		ConfirmationWidget->AddToViewport();
	}
}

void UPauseWidget::LoadCheckPoint()
{
	CloseConfirmationWidget();
	const auto GameInstance = GetWorld()->GetGameInstance<UPRGameInstance>();
	
	UGameplayStatics::OpenLevel(this,/*GameInstance->GetStartupLevel().LevelName*/"LVL_Conference");
}

void UPauseWidget::PlayAgain()
{
	CloseConfirmationWidget();
	/*const auto GameInstance = GetWorld()->GetGameInstance<UPRGameInstance>();
	UGameplayStatics::DeleteGameInSlot("SaveSlot",0);*/
	auto SaveGame = Cast<UPRSaveGame>(UGameplayStatics::CreateSaveGameObject(UPRSaveGame::StaticClass()));
	const auto OldSaveGame = Cast<UPRSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveSlot",0));
	SaveGame->GameDifficulty = OldSaveGame->GameDifficulty;
	SaveGame->InitialSave = true;
	UGameplayStatics::DeleteGameInSlot("SaveSlot",0);
	UGameplayStatics::SaveGameToSlot(SaveGame,"SaveSlot",0);
	UGameplayStatics::OpenLevel(this,/*GameInstance->GetStartupLevel().LevelName*/"LVL_Conference");
	
}

void UPauseWidget::QuitGame()
{
	CloseConfirmationWidget();
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UPauseWidget::CloseConfirmationWidget()
{
	if (ConfirmationWidget)
	{
		ConfirmationWidget->RemoveFromParent();
	}
}
