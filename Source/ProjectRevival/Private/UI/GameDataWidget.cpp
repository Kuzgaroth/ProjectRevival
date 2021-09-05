// Project Revival. All Rights Reserved


#include "UI/GameDataWidget.h"
#include "PRGameModeBase.h"
#include "PRPlayerState.h"

int32 UGameDataWidget::GetKillsNum() const
{
	const auto PlayerState = GetPlayerState();
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 UGameDataWidget::GetDeathsNum() const
{
	const auto PlayerState = GetPlayerState();
	return PlayerState ? PlayerState->GetDeathsNum() : 0;
}

int32 UGameDataWidget::GetTotalRoundsNum() const
{
	const auto GameMode = GetGameMode();
	return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 UGameDataWidget::GetCurrentRoundNum() const
{
	const auto GameMode = GetGameMode();
	return GameMode ? GameMode->GetCurrentRoundNum(): 0;
}

int32 UGameDataWidget::GetRoundSecondsRemaining() const
{
	const auto GameMode = GetGameMode();
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

APRGameModeBase* UGameDataWidget::GetGameMode() const
{
	return GetWorld() ? Cast<APRGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

APRPlayerState* UGameDataWidget::GetPlayerState() const 
{
	return GetOwningPlayer() ? Cast<APRPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
