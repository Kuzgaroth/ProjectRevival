// Project Revival. All Rights Reserved


#include "AI/Soldier/Tasks/BTT_StartCoverTimer.h"

#include "Soldier/SoldierAIController.h"

UBTT_StartCoverTimer::UBTT_StartCoverTimer()
{
	NodeName = "Start Cover Timer";
}

EBTNodeResult::Type UBTT_StartCoverTimer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return EBTNodeResult::Failed;
	Controller->StartCoverTimer();
	return EBTNodeResult::Succeeded;
}
