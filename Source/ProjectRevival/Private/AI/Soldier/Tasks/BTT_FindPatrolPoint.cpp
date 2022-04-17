// Project Revival. All Rights Reserved


#include "AI/Soldier/Tasks/BTT_FindPatrolPoint.h"

#include "Soldier/SoldierAIController.h"

UBTT_FindPatrolPoint::UBTT_FindPatrolPoint()
{
	NodeName = "Find Patrol Path";
}

EBTNodeResult::Type UBTT_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return EBTNodeResult::Failed;

	EBTNodeResult::Type Result;

	UE_LOG(LogPRAITasks, Warning, TEXT("bIsPatrolling: %s"), Controller->GetBIsPatrolling()?TEXT("true"):TEXT("false"))
	if (!Controller->GetBIsPatrolling())
	{
		if (Controller->FindPatrolPath())
		{
			Result = EBTNodeResult::Succeeded;
		}
		else
		{
			Result = EBTNodeResult::Failed;
		}
	}
	else
	{
		Controller->FindNextPatrolPoint();
		Result = EBTNodeResult::Succeeded;
	}
	return Result;
}
