// Project Revival. All Rights Reserved


#include "AI/Soldier/Tasks/BTT_EnterCover.h"

#include "Soldier/SoldierAIController.h"

UBTT_EnterCover::UBTT_EnterCover()
{
	NodeName = "Enter cover";
}

EBTNodeResult::Type UBTT_EnterCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return EBTNodeResult::Failed;

	Controller->StartEnteringCover();
	return EBTNodeResult::Succeeded;
}
