// Project Revival. All Rights Reserved


#include "AI/Soldier/Decorators/BTD_CheckForWingSide.h"

UBTD_CheckForWingSide::UBTD_CheckForWingSide()
{
	NodeName = "Check for wing side";
}

bool UBTD_CheckForWingSide::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return CalcCondition(OwnerComp, NodeMemory);
}

void UBTD_CheckForWingSide::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	DecoratorMemory->bLastRawResult = CalcCondition(OwnerComp, NodeMemory);
}

void UBTD_CheckForWingSide::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	
	const bool bResult = CalcCondition(OwnerComp, NodeMemory);
	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
}

bool UBTD_CheckForWingSide::CalcCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return true;
}
