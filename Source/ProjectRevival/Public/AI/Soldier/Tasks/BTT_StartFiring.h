// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StartFiring.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTT_StartFiring : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_StartFiring();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
