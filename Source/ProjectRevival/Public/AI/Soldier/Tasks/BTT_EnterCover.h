// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EnterCover.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTT_EnterCover : public UBTTaskNode
{
	GENERATED_BODY()

	UBTT_EnterCover();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
