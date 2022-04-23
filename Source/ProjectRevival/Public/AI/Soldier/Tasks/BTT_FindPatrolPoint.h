// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FindPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTT_FindPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	UBTT_FindPatrolPoint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
