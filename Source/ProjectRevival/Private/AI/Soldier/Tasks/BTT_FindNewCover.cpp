// Project Revival. All Rights Reserved


#include "AI/Soldier/Tasks/BTT_FindNewCover.h"

#include "PRAIPerceptionComponent.h"
#include "AI/Soldier/SoldierAIController.h"
#include "ProjectRevival/Public/CoreTypes.h"

UBTT_FindNewCover::UBTT_FindNewCover()
{
	NodeName = "Find New Cover";
}

EBTNodeResult::Type UBTT_FindNewCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return EBTNodeResult::Failed;
	
	Controller->FindNewCover();
	UE_LOG(LogPRAIDecorators, Log, TEXT("Cover is in search"));
	return EBTNodeResult::Succeeded;
}
