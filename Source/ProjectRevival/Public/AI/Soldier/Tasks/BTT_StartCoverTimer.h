// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StartCoverTimer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTT_StartCoverTimer : public UBTTaskNode
{
	GENERATED_BODY()
	UBTT_StartCoverTimer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
