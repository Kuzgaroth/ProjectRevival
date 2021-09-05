// Project Revival. All Rights Reserved


#include "UI/GameOverWidget.h"
#include "PRGameModeBase.h"
#include "Components/VerticalBox.h"
#include "ProjectRevival/Public/Player/PRPlayerState.h"
#include "ProjectRevival/Public/UI/PlayerStatRowWidget.h"
#include "PRUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (GetWorld())
	{
		const auto GameMode = Cast<APRGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &UGameOverWidget::OnMatchStateChanged);
		}
	}
	if (ResetButton)
	{
		ResetButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnResetLevel);
	}
}

void UGameOverWidget::OnMatchStateChanged(EMatchState NewMatchState)
{
	if (NewMatchState == EMatchState::GameOver)
	{
		UpdatePlayersStat();
	}
}

void UGameOverWidget::UpdatePlayersStat()
{
	if (!GetWorld() || !PLayerStatBox) return;

	PLayerStatBox->ClearChildren();
	
	for (auto It = GetWorld()->GetControllerIterator(); It;++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<APRPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		const auto PlayerStatRowWidget = CreateWidget<UPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
		if (!PlayerStatRowWidget) continue;

		PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
		PlayerStatRowWidget->SetKills(PRUtils::TextFromInt(PlayerState->GetKillsNum()));
		PlayerStatRowWidget->SetDeaths(PRUtils::TextFromInt(PlayerState->GetDeathsNum()));
		PlayerStatRowWidget->SetTeam(PRUtils::TextFromInt(PlayerState->GetTeamId()));
		PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());

		PLayerStatBox->AddChild(PlayerStatRowWidget);
	}
}

void UGameOverWidget::OnResetLevel()
{
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
