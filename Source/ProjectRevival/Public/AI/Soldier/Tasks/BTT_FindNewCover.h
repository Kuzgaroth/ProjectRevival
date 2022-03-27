// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FindNewCover.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTT_FindNewCover : public UBTTaskNode
{
	GENERATED_BODY()
	UBTT_FindNewCover();

	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FBlackboardKeySelector AimLocationKey;

};
