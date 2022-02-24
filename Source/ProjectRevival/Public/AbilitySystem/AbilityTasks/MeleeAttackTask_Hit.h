// Project Revival. All Rights Reserved

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "AssassinEnemy.h"
#include "MeleeWeapon.h"
#include "MeleeAttackTask_Hit.generated.h"

UCLASS()
class PROJECTREVIVAL_API UMeleeAttackTask_Hit : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UMeleeAttackTask_Hit* AttackInit(UGameplayAbility* OwningAbility, UCurveFloat* AttackCurve,
		UAnimMontage* Montage, float PlayRate, float AttackDuration);
	UFUNCTION()
	void AttackStarted();
	UFUNCTION()
	void AttackFinished();
	
	UFUNCTION()
	void TickTimeline(float Delta);
	
private:
	virtual void OnDestroy(bool AbilityEnded) override;

	UPROPERTY(EditDefaultsOnly)
	float AttackDuration;
	UPROPERTY(EditDefaultsOnly)
	float PlayRate;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* MeleeAttackMontage;
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* CurveFloat;

	UPROPERTY()
	class UAbilityTask_PlayMontageAndWait* MontageTask;
	UPROPERTY()
	class UAbilityTask_WaitDelay* DelayTask;
	
	FTimeline Timeline;
};