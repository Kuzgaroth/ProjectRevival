// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FindEnemyInSight.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UFindEnemyInSight : public UBTService
{
	GENERATED_BODY()
public:
	UFindEnemyInSight();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
