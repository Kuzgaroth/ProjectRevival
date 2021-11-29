// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Components/TimelineComponent.h"
#include "FuncActors/TickActor.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "FlipTask_FlipToggle.generated.h"


UCLASS()
class PROJECTREVIVAL_API UFlipTask_FlipToggle : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;
	void Activate(float Strength, float Duration, UCurveFloat* Curve);

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FlipMontage;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFlipTask_FlipToggle* FlipInit(UGameplayAbility* OwningAbility, UCurveFloat* FlipCurve);
	
	void FlipStarted(float Strength, float Duration, UCurveFloat* Curve);
	void FlipFinished();
	FOnTimelineEvent OnFlipStarted;
	FOnTimelineEvent OnFlipFinished;
	
	UFUNCTION()
	void TickTimeline(float Delta);
private:
	virtual void OnDestroy(bool AbilityEnded) override;
	
	UPROPERTY()
	ATickActor* TickActor;
	
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* CurveFloat;

	UPROPERTY()
	class UAbilityTask_ApplyRootMotionConstantForce* ForceTask;
	
	FTimeline Timeline;
};
