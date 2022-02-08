// Project Revival. All Rights Reserved

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "MeleeAttackTask_Hit.generated.h"

UCLASS()
class PROJECTREVIVAL_API UMeleeAttackTask_Hit : public UAbilityTask
{
	GENERATED_BODY()
public:
	void Activate() override;
	void Activate(float Duration, UCurveFloat* Curve, UAnimMontage* MeleeAttackMontage);
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UMeleeAttackTask_Hit* AttackInit(UGameplayAbility* OwningAbility, UCurveFloat* AttackCurve);
	
	void AttackStarted(float Duration, UCurveFloat* Curve, UAnimMontage* MeleeAttackMontage);
	void AttackFinished();
	FOnTimelineEvent OnAttackStarted;
	FOnTimelineEvent OnAttackFinished;
	
	UFUNCTION()
	void TickTimeline(float Delta);
private:
	virtual void OnDestroy(bool AbilityEnded) override;
	
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* CurveFloat;

	UPROPERTY()
	class UAbilityTask_PlayMontageAndWait* MontageTask;
	
	FTimeline Timeline;
};