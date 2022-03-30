// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "BTD_DecisionMakingAllowed.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTD_DecisionMakingAllowed : public UBTDecorator
{
	GENERATED_BODY()
	typedef FBTPlayerCheckDecoratorMemory TNodeInstanceMemory;

	UBTD_DecisionMakingAllowed();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CalcCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	
};
