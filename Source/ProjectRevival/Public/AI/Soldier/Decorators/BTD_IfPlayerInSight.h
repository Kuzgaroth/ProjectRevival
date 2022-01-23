// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IfPlayerInSight.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPRAIDecorators, Log, All);

struct FBTPlayerCheckDecoratorMemory
{
	bool bLastRawResult;
};

/*
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTD_IfPlayerInSight : public UBTDecorator
{
	GENERATED_BODY()

	typedef FBTPlayerCheckDecoratorMemory TNodeInstanceMemory;
	
	UBTD_IfPlayerInSight();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CalcCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
