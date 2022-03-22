// Project Revival. All Rights Reserved


#include "AI/Soldier/Decorators/BTD_IfBotInCover.h"
#include "Soldier/SoldierAIController.h"



UBTD_IfBotInCover::UBTD_IfBotInCover()
{
	NodeName = "If Bot Is In Cover";
}

bool UBTD_IfBotInCover::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return CalcCondition(OwnerComp, NodeMemory);
}

void UBTD_IfBotInCover::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	DecoratorMemory->bLastRawResult = CalcCondition(OwnerComp, NodeMemory);
}

void UBTD_IfBotInCover::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	
	const bool bResult = CalcCondition(OwnerComp, NodeMemory);
	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
}

bool UBTD_IfBotInCover::CalcCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return false;

	if (Controller->GetBIsInCover())
	{
		return true;
	}
	else
	{
		return false;
	}
}
