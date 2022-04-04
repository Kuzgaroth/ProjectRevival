// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "BTD_CheckForBotState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTD_CheckForBotState : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_CheckForBotState();

	typedef FBTPlayerCheckDecoratorMemory TNodeInstanceMemory;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CalcCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
