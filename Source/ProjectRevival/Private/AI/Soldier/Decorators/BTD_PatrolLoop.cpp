// Project Revival. All Rights Reserved


#include "AI/Soldier/Decorators/BTD_PatrolLoop.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "Soldier/SoldierAIController.h"

UBTD_PatrolLoop::UBTD_PatrolLoop()
{
	NodeName = "Patrol Loop";
	bNotifyDeactivation = true;

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;
}

bool UBTD_PatrolLoop::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	return Controller->GetBIsPatrolling() && !Controller->GetBIsPlayerInSight();
}

EBlackboardNotificationResult UBTD_PatrolLoop::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard,
	FBlackboard::FKey ChangedKeyID)
{
	return EBlackboardNotificationResult::RemoveObserver;
}

void UBTD_PatrolLoop::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	if (NodeResult != EBTNodeResult::Aborted)
	{
		const UBlackboardComponent* BlackboardComp = SearchData.OwnerComp.GetBlackboardComponent();
		const auto Controller = Cast<ASoldierAIController>(SearchData.OwnerComp.GetAIOwner());
		const bool bEvalResult = BlackboardComp && EvaluateOnBlackboard(*BlackboardComp) &&
				Controller->GetBIsPatrolling() && !Controller->GetBIsPlayerInSight();
		
		if (bEvalResult != IsInversed())
		{
			GetParentNode()->SetChildOverride(SearchData, GetChildIndex());
		}
	}
}
