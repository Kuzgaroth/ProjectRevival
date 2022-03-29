// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BTDecorator.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "BTD_CheckForWingSide.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UBTD_CheckForWingSide : public UBTDecorator
{
	GENERATED_BODY()
	typedef FBTPlayerCheckDecoratorMemory TNodeInstanceMemory;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Comparekey;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	EWing Wing;

	UBTD_CheckForWingSide();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CalcCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
