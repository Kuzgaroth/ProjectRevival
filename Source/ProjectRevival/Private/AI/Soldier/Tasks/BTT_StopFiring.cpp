// Project Revival. All Rights Reserved


#include "AI/Soldier/Tasks/BTT_StopFiring.h"
#include "Soldier/SoldierAIController.h"

UBTT_StopFiring::UBTT_StopFiring()
{
	NodeName = "Stop Firing At Player";
}

EBTNodeResult::Type UBTT_StopFiring::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if(!Controller) return EBTNodeResult::Failed;
	
	Controller->SetBIsFiring(false);
	return EBTNodeResult::Succeeded;
}
