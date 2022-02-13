// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/ChangeWorldTask_SpawnSphere.h"
#include "ChangeWorldAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UChangeWorldAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
	protected:
		virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
    	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	private:
	UPROPERTY()
	UChangeWorldTask_SpawnSphere* ChangeWorldTask;
	UPROPERTY()
	TSubclassOf<AActor> ChangeWorldShere;
	UPROPERTY()
	float TraceSpawnDistance=1500.0f;
	
};
