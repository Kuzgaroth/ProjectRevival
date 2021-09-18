// Project Revival. All Rights Reserved


#include "States/TemporaryState.h"

UTemporaryState::UTemporaryState()
{
}

UTemporaryState::~UTemporaryState()
{
	StateTimeFinishedDelegate.Clear();
	StateInterraptedDelegate.Clear();
}

void UTemporaryState::OnStateTimeEnded()
{
	IsInProgress = false;
	StateTimeFinishedDelegate.Broadcast();
}

void UTemporaryState::EndState()
{
	//StateTimeFinishedDelegate.Clear();
	if (IsInProgress)
	{
		StateInterraptedDelegate.Broadcast();
		IsInProgress = false;
	}
	else
	{
		StateEndedDelegate.Broadcast();
	}
}

void UTemporaryState::StartState()
{
	Super::StartState();
	IsInProgress = true;
}
