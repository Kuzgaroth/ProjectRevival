// Project Revival. All Rights Reserved


#include "AI/Soldier/Decorators/BTD_CheckForWingSide.h"

UBTD_CheckForWingSide::UBTD_CheckForWingSide()
{
}

bool UBTD_CheckForWingSide::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}

void UBTD_CheckForWingSide::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTD_CheckForWingSide::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

bool UBTD_CheckForWingSide::CalcCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return false;
}
