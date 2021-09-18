// Project Revival. All Rights Reserved


#include "States/PermanentState.h"


UPermanentState::UPermanentState()
{
}

UPermanentState::~UPermanentState()
{
	StateEndedDelegate.Clear();
	StateStartDelegate.Clear();
}

void UPermanentState::StartState()
{
	StateStartDelegate.Broadcast();
}

void UPermanentState::EndState()
{
	StateEndedDelegate.Broadcast();
}

bool UPermanentState::IsStateFriendly(UPermanentState* CheckedState) const
{
	return FriendlyStates.Contains(CheckedState);
}

void UPermanentState::SetFriendlyStates(TArray<UPermanentState*>& Friends)
{
	FriendlyStates.Empty();
	FriendlyStates.Append(Friends);
}