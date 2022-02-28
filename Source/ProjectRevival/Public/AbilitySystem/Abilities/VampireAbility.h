// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/FlipTask_FlipToggle.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "VampireAbility.generated.h"

UCLASS()
class PROJECTREVIVAL_API UVampireAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UVampireAbility();
	    
	UPROPERTY(EditDefaultsOnly, Category="Flip constants")
	float VampireAbilityDistance;
    
	UPROPERTY(EditDefaultsOnly, Category="Flip constants")
	float VampireAbilityDamage;
	
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* VampireMontage;	
protected:	
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	UFUNCTION()
	void OnTraceAnalysisEnd();
private:
	UPROPERTY()
	UFlipTask_FlipToggle* TraceTask;
	UPROPERTY()
	UAbilityTask_WaitDelay* DelayTask;
	UPROPERTY()
	class UAbilityTask_PlayMontageAndWait* MontageTask;
};