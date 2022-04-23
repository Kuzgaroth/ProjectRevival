// Project Revival. All Rights Reserved


#include "Menu/DifficultySelectionWidget.h"



#include "PRGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/MenuGameModeBase.h"
#include "Menu/MenuWidget.h"
#include "SaveSystem/PRSaveGame.h"

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
	SetDifficulty(Easy);
}

void UDifficultySelectionWidget::OnMediumDifficultyPressed()
{
	SetDifficulty(Normal);
}

void UDifficultySelectionWidget::OnHighDifficultyPressed()
{
	SetDifficulty(Hard);
}

void UDifficultySelectionWidget::SetDifficulty(EGameDifficulty GameDifficulty)
{
	// Save difficulty level here
	auto SaveGame = Cast<UPRSaveGame>(UGameplayStatics::CreateSaveGameObject(UPRSaveGame::StaticClass()));
	SaveGame->GameDifficulty = GameDifficulty;
	SaveGame->InitialSave = true;
	UGameplayStatics::SaveGameToSlot(SaveGame,"SaveSlot",0);
	OpenLevel();
}

void UDifficultySelectionWidget::OpenLevel()
{
	const auto GameInstance = GetWorld()->GetGameInstance<UPRGameInstance>();
	
	//const auto MenuGameMode = GetWorld()->GetAuthGameMode<AMenuGameModeBase>();
	//if (MenuGameMode) MenuGameMode->ClearSaveData();
	UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
	UGameplayStatics::OpenLevel(this,/*GameInstance->GetStartupLevel().LevelName*/"LVL_Conference");
	
	// const auto GameInstance = GetWorld()->GetGameInstance<UPRGameInstance>();
	// UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
	// UGameplayStatics::OpenLevel(this, GameInstance->GetLevelsData()[0].LevelName);
}
