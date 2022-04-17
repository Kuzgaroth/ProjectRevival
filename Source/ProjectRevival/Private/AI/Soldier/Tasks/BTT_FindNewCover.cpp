// Project Revival. All Rights Reserved


#include "AI/Soldier/Tasks/BTT_FindNewCover.h"
#include "AI/Soldier/SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Interfaces/IChangingWorldActor.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectRevival/Public/CoreTypes.h"

UBTT_FindNewCover::UBTT_FindNewCover()
{
	NodeName = "Find New Cover";
}

EBTNodeResult::Type UBTT_FindNewCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return EBTNodeResult::Failed;
	UE_LOG(LogPRAIDecorators, Log, TEXT("Cover is in search"))
	if (!Controller->FindNewCover()) return EBTNodeResult::Failed;
	return EBTNodeResult::Succeeded;
}
