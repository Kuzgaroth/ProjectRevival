// Project Revival. All Rights Reserved

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "CoreMinimal.h"
#include "VampireAbility_TraceTask.generated.h"

UCLASS()
class PROJECTREVIVAL_API UVampireAbility_TraceTask : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UVampireAbility_TraceTask* TaskInit(UGameplayAbility* OwningAbility, float Distance);

	UPROPERTY(BlueprintReadOnly)
	bool Status = false;
	
	UPROPERTY()
	FHitResult HitResult;
	
private:
	void TraceAnalysisStarted();
	
	virtual void OnDestroy(bool bAbilityEnded) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Flip constants")
	float VampireAbilityDistance;
};
