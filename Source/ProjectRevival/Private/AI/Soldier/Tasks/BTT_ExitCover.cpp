// Project Revival. All Rights Reserved


#include "AI/Soldier/Tasks/BTT_ExitCover.h"

#include "Soldier/SoldierAIController.h"

UBTT_ExitCover::UBTT_ExitCover()
{
	NodeName = "Exit Cover";
}

EBTNodeResult::Type UBTT_ExitCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return EBTNodeResult::Failed;

	Controller->StartExitingCover();
	return EBTNodeResult::Succeeded;
}
