// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ExitCover.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTT_ExitCover : public UBTTaskNode
{
	GENERATED_BODY()

	UBTT_ExitCover();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
