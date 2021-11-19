// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "VisorAbility.generated.h"



UCLASS()
class PROJECTREVIVAL_API UVisorAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UVisorAbility();
protected:
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
