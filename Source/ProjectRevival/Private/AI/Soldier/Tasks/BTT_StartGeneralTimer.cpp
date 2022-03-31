// Project Revival. All Rights Reserved


#include "AI/Soldier/Tasks/BTT_StartGeneralTimer.h"

#include "Soldier/SoldierAIController.h"

UBTT_StartGeneralTimer::UBTT_StartGeneralTimer()
{
	NodeName = "Start general timer";
}

EBTNodeResult::Type UBTT_StartGeneralTimer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return EBTNodeResult::Failed;
	Controller->StartGeneralTimer();
	return EBTNodeResult::Succeeded;
}
