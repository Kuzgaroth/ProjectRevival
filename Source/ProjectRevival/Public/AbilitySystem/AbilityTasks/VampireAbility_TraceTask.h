// Project Revival. All Rights Reserved

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "FuncActors/TickActor.h"
#include "VampireAbility_TraceTask.generated.h"

UCLASS()
class PROJECTREVIVAL_API UVampireAbility_TraceTask : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* VampireMontage;

	UPROPERTY(EditDefaultsOnly, Category="Flip constants")
	float VampireAbilityDistance;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UVampireAbility_TraceTask* TaskInit(UGameplayAbility* OwningAbility, UCurveFloat* FlipCurve,
		float Strength, float Duration, UAnimMontage* Montage, FVector Direction);
	
private:
	void TraceAnalysisStarted();
	void TraceAnalysisFinished();
	FOnTimelineEvent OnAnalysisStarted;
	FOnTimelineEvent OnAnalysisFinished;
	
    UPROPERTY()
    class UAbilityTask_PlayMontageAndWait* MontageTask;
	
	UFUNCTION()
	void TickTimeline(float Delta);
	FTimeline Timeline;

	virtual void OnDestroy(bool bAbilityEnded) override;
};
