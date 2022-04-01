// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StartGeneralTimer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTT_StartGeneralTimer : public UBTTaskNode
{
	GENERATED_BODY()
	UBTT_StartGeneralTimer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
