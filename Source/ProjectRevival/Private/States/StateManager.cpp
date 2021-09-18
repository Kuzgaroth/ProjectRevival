﻿// Project Revival. All Rights Reserved


#include "States/StateManager.h"
#include "ProjectRevival/Public/States/PermanentState.h"
#include "States/TemporaryState.h"

UStateManager::UStateManager()
{
}

UStateManager::~UStateManager()
{
	
}

void UStateManager::OnStateTimeFinished()
{
	EnterDefaultState();
}

bool UStateManager::TryEnterState(UPermanentState* NewState)
{
	if (CurrentState->IsStateFriendly(NewState))
	{
		CurrentState->EndState();
		CurrentState = NewState;
		if (NewState->IsA<UTemporaryState>())
			Cast<UTemporaryState>(NewState)->StateTimeFinishedDelegate.AddUObject(this,
				&UStateManager::OnStateTimeFinished);
		NewState->StartState();
		return true;
	}
	return false;
}

void UStateManager::EnterState(UPermanentState* NewState)
{
	if (CurrentState->IsStateFriendly(NewState))
	{
		CurrentState->EndState();
		CurrentState = NewState;
		if (NewState->IsA<UTemporaryState>())
			Cast<UTemporaryState>(NewState)->StateTimeFinishedDelegate.AddUObject(this,
				&UStateManager::OnStateTimeFinished);
		NewState->StartState();
	}
}

void UStateManager::SetDefaultState(UPermanentState* DefState)
{
	this->DefaultState = DefState;
}

void UStateManager::EnterDefaultState()
{
	EnterState(DefaultState);
}
