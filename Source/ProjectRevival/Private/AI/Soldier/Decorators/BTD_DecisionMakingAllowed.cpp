// Project Revival. All Rights Reserved


#include "AI/Soldier/Decorators/BTD_DecisionMakingAllowed.h"

#include "Soldier/SoldierAIController.h"

UBTD_DecisionMakingAllowed::UBTD_DecisionMakingAllowed()
{
	NodeName = "Is decision making allowed";
}

bool UBTD_DecisionMakingAllowed::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return CalcCondition(OwnerComp, NodeMemory);
}

void UBTD_DecisionMakingAllowed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	DecoratorMemory->bLastRawResult = CalcCondition(OwnerComp, NodeMemory);
}

void UBTD_DecisionMakingAllowed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	
	const bool bResult = CalcCondition(OwnerComp, NodeMemory);
	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
}

bool UBTD_DecisionMakingAllowed::CalcCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return false;

	UE_LOG(LogPRAIDecorators, Log, TEXT("%b"), Controller->GetBIsDecisionMakingAllowed());
	return Controller->GetBIsDecisionMakingAllowed() && Controller->GetBIsFiringAllowed();
}
