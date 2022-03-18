// Project Revival. All Rights Reserved

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "FuncActors/TickActor.h"
#include "FlipTask_FlipToggle.generated.h"

UCLASS()
class PROJECTREVIVAL_API UFlipTask_FlipToggle : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FlipMontage;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFlipTask_FlipToggle* FlipInit(UGameplayAbility* OwningAbility, UCurveFloat* FlipCurve,
		float Strength, float Duration, UAnimMontage* Montage, FVector Direction);
	
	void FlipStarted();
	void FlipFinished();
	FOnTimelineEvent OnFlipStarted;
	FOnTimelineEvent OnFlipFinished;
	
	UFUNCTION()
	void TickTimeline(float Delta);
private:
	virtual void OnDestroy(bool AbilityEnded) override;
	
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* CurveFloat;
	UPROPERTY(EditDefaultsOnly)
	float FlipStrength;
	UPROPERTY(EditDefaultsOnly)
	float FlipDuration;
	UPROPERTY(EditDefaultsOnly)
	FVector FlipDirection;
	
	UPROPERTY()
	class UAbilityTask_ApplyRootMotionConstantForce* ForceTask;

	UPROPERTY()
	class UAbilityTask_PlayMontageAndWait* MontageTask;
	
	FTimeline Timeline;
};
