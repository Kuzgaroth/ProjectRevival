// Project Revival. All Rights Reserved


#include "Menu/MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectRevival/Public/PRGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/CreditsWidget.h"
#include "Menu/DifficultySelectionWidget.h"
#include "Menu/OptionsWidget.h"
#include "ProjectRevival/Public/Menu/LevelItemWidget.h"
#include "Sound/SoundCue.h"

void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGame);
		StartGameButton->OnHovered.AddDynamic(this, &UMenuWidget::OnStartGameHovered);
		StartGameButton->OnUnhovered.AddDynamic(this, &UMenuWidget::OnStartGameUnhovered);
	}
	
	if (ContinueGameButton)
	{
		ContinueGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnContinueGame);
	}

	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UMenuWidget::OnOptions);
	}
	
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitGame);
	}

	if (CreditsButton)
	{
		CreditsButton->OnClicked.AddDynamic(this, &UMenuWidget::OnCredits);
	}

	// InitLevelItems();
}

void UMenuWidget::OnStartGame()
{
	// const auto GameInstance = GetWorld()->GetGameInstance<UPRGameInstance>();
	// UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
	// UGameplayStatics::OpenLevel(this,GameInstance->GetStartupLevel().LevelName);
	//
	if (DifficultySelectorWidgetClass)
	{
		RemoveFromParent();
		UDifficultySelectionWidget* DifficultySelectionWidget = CreateWidget<UDifficultySelectionWidget>(GetWorld(), DifficultySelectorWidgetClass);
		DifficultySelectionWidget->AddToViewport();
	}
	
}

void UMenuWidget::OnContinueGame()
{
	
}

void UMenuWidget::OnOptions()
{
	if (OptionsWidgetClass)
	{
		LeaveEvent();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMenuWidget::OpenOptions, 1.0f, false, 0.125f);
	}
}

void UMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMenuWidget::OnCredits()
{
	if (CreditsWidgetClass)
	{
		LeaveEvent();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMenuWidget::OpenCredits, 1.0f, false, 0.125f);
	}
}

void UMenuWidget::OpenOptions()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	if (OptionsWidgetClass)
	{
		RemoveFromParent();
		UOptionsWidget* OptionsWidget = CreateWidget<UOptionsWidget>(GetWorld(), OptionsWidgetClass);
		OptionsWidget->AddToViewport();
	}
}

void UMenuWidget::OpenCredits()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	if (CreditsWidgetClass)
	{
		RemoveFromParent();
		UCreditsWidget* CreditsWidget = CreateWidget<UCreditsWidget>(GetWorld(), CreditsWidgetClass);
		CreditsWidget->AddToViewport();
	}
}

void UMenuWidget::InitLevelItems()
{
	const auto GameInstance = GetGameInstance<UPRGameInstance>();

	checkf(GameInstance->GetLevelsData().Num() != 0, TEXT("Empty levels"));

	if (!LevelsBox) return;
	LevelsBox->ClearChildren();

	for (auto LevelData : GameInstance->GetLevelsData())
	{
		const auto LevelWidget = CreateWidget<ULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		if (!LevelWidget) continue;

		LevelWidget->SetLevelData(LevelData);
		LevelWidget->OnLevelSelected.AddUObject(this, &UMenuWidget::OnLevelSelected);

		LevelsBox->AddChild(LevelWidget);
		LevelItemWidgets.Add(LevelWidget);
	}
	if (GameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected(GameInstance->GetLevelsData()[0]);
	}
	else
	{
		OnLevelSelected(GameInstance->GetStartupLevel());
	}
}

void UMenuWidget::OnLevelSelected(const FLevelData& NewLevelData)
{
	const auto GameInstance = GetGameInstance<UPRGameInstance>();
	GameInstance->SetStartupLevel(NewLevelData);
	for (auto LevelItemWidget : LevelItemWidgets)
	{
		if (LevelItemWidget)
		{
			const auto IsSelected = NewLevelData.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(IsSelected);
		}
	}
}
