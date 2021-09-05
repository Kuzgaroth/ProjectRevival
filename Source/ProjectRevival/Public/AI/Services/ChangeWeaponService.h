// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ChangeWeaponService.generated.h"



UCLASS()
class PROJECTREVIVAL_API UChangeWeaponService : public UBTService
{
	GENERATED_BODY()
public:
	UChangeWeaponService();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float Probability = 0.5f;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
