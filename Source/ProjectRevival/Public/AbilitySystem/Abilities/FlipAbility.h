// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/FlipTask_FlipToggle.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "FlipAbility.generated.h"

UCLASS()
class PROJECTREVIVAL_API UFlipAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UFlipAbility();
	
	UPROPERTY(EditDefaultsOnly, Category="Flip Visual")
	UCurveFloat* FlipCurve;
    
	UPROPERTY(EditDefaultsOnly, Category="Flip Duration")
	float FlipDuration;
    
	UPROPERTY(EditDefaultsOnly, Category="Flip Strength")
	float FlipStrength;
	
protected:	
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	UFUNCTION()
	void OnFlipBegin();
	UFUNCTION()
	void OnFlipEnd();
	
private:
	UPROPERTY()
	UFlipTask_FlipToggle* FlipTask;
	UPROPERTY()
	UAbilityTask_WaitDelay* DelayTask;
};
