// Project Revival. All Rights Reserved


#include "Menu/DifficultySelectionWidget.h"



#include "PRGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/MenuGameModeBase.h"
#include "Menu/MenuWidget.h"

void UDifficultySelectionWidget::NativeOnInitialized()
{
	if (LowDifficultyWidget)
	{
		LowDifficultyWidget->DifficultyButton->OnClicked.AddDynamic(this, &UDifficultySelectionWidget::OnLowDifficultyPressed);
	}
	if (MediumDifficultyWidget)
	{
		MediumDifficultyWidget->DifficultyButton->OnClicked.AddDynamic(this, &UDifficultySelectionWidget::OnMediumDifficultyPressed);
	}
	if (HighDifficultyWidget)
	{
		HighDifficultyWidget->DifficultyButton->OnClicked.AddDynamic(this, &UDifficultySelectionWidget::OnHighDifficultyPressed);
	}
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UDifficultySelectionWidget::OnBack);
	}
}

void UDifficultySelectionWidget::OnBack()
{
	if (MenuWidgetClass)
	{
		RemoveFromParent();
		UMenuWidget* MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuWidgetClass);
		MenuWidget->AddToViewport();
	}
}

void UDifficultySelectionWidget::OnLowDifficultyPressed()
{
	SetDifficulty("Low");
}

void UDifficultySelectionWidget::OnMediumDifficultyPressed()
{
	SetDifficulty("Medium");
}

void UDifficultySelectionWidget::OnHighDifficultyPressed()
{
	SetDifficulty("High");
}

void UDifficultySelectionWidget::SetDifficulty(FString DifficultyName)
{
	// Save difficulty level here

	OpenLevel();
}

void UDifficultySelectionWidget::OpenLevel()
{
	const auto GameInstance = GetWorld()->GetGameInstance<UPRGameInstance>();
	
	const auto MenuGameMode = GetWorld()->GetAuthGameMode<AMenuGameModeBase>();
	if (MenuGameMode) MenuGameMode->ClearSaveData();
	UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
	UGameplayStatics::OpenLevel(this,/*GameInstance->GetStartupLevel().LevelName*/"LVL_Conference");
	
	// const auto GameInstance = GetWorld()->GetGameInstance<UPRGameInstance>();
	// UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
	// UGameplayStatics::OpenLevel(this, GameInstance->GetLevelsData()[0].LevelName);
}
