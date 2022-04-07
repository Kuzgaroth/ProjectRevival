// Project Revival. All Rights Reserved


#include "AI/Soldier/Decorators/BTD_CheckForBotState.h"

#include "Soldier/SoldierAIController.h"

UBTD_CheckForBotState::UBTD_CheckForBotState()
{
	NodeName = "Check for Bot state";
}

bool UBTD_CheckForBotState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return CalcCondition(OwnerComp, NodeMemory);
}

void UBTD_CheckForBotState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	DecoratorMemory->bLastRawResult = CalcCondition(OwnerComp, NodeMemory);
}

void UBTD_CheckForBotState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	
	const bool bResult = CalcCondition(OwnerComp, NodeMemory);
	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
}

bool UBTD_CheckForBotState::CalcCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return false;
	UE_LOG(LogPRAIDecorators, Log, TEXT("Current bot state is: %s"), Controller->GetBotState() == EBotState::Battle ? TEXT("Battle") : TEXT("Idle"))
	return Controller->GetBotState() == EBotState::Battle;
}
