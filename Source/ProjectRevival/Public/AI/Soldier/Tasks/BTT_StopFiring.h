// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StopFiring.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTT_StopFiring : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_StopFiring();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
