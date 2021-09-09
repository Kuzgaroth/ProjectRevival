// Project Revival. All Rights Reserved


#include "States/PermanentState.h"


UPermanentState::UPermanentState()
{
}

UPermanentState::~UPermanentState()
{
}

void UPermanentState::StartState()
{
}

void UPermanentState::EndState()
{
	StateEndedDelegate.Broadcast();
}

bool UPermanentState::IsStateFriendly(UPermanentState* CheckedState)
{
	return FriendlyStates.Contains(CheckedState);
}

void UPermanentState::SetFriendlyStates(TArray<UPermanentState*>& Friends)
{
	FriendlyStates.Empty();
	FriendlyStates.Append(Friends);
}