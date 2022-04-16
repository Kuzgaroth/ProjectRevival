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
public:
	UChangeWorldAbility();
	
	protected:
		virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
    	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	private:
	UPROPERTY()
	UChangeWorldTask_SpawnSphere* ChangeWorldTask;
	UPROPERTY(EditAnywhere,Category="Sphere Actor")
	TSubclassOf<AActor> ChangeWorldShere;
	UPROPERTY(EditAnywhere,Category="Trace Params")
	float TraceSpawnDistance=1500.0f;
	UPROPERTY(EditAnywhere,Category="Trace Params")
	bool FreezePlayerDurindAbility=false;
	void FinishAbility();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
};
