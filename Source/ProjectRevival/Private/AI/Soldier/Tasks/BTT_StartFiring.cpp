// Project Revival. All Rights Reserved


#include "AI/Soldier/Tasks/BTT_StartFiring.h"
#include "AI/AICharacter.h"
#include "Soldier/SoldierAIController.h"

UBTT_StartFiring::UBTT_StartFiring()
{
	NodeName = "Start Firing At Player";
}

EBTNodeResult::Type UBTT_StartFiring::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if(!Controller) return EBTNodeResult::Failed;

	const auto Character = Cast<AAICharacter>(Controller->GetPawn());
	if(!Character) return EBTNodeResult::Failed;

	Controller->StartFiring();
	return EBTNodeResult::Succeeded;
}
