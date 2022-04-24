// Project Revival. All Rights Reserved


#include "CourtLairGameMode.h"

ACourtLairGameMode::ACourtLairGameMode()
{
}

void ACourtLairGameMode::SetMatchState(EMatchState NewMatchState)
{
	if (this->MatchState==NewMatchState) return;
	this->MatchState = NewMatchState;

	OnMatchStateChanged.Broadcast(this->MatchState);
}

void ACourtLairGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetMatchState(EMatchState::InProgress);
}

bool ACourtLairGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);;
	if (PauseSet)
	{
		SetMatchState(EMatchState::Pause);
	}
	return PauseSet;
}

bool ACourtLairGameMode::ClearPause()
{
	const bool PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		SetMatchState(EMatchState::InProgress);
	}
	return PauseCleared;
}
