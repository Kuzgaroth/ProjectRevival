// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "FlipAbility.generated.h"



UCLASS()
class PROJECTREVIVAL_API UFlipAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UFlipAbility();
protected:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FlipMontage;
	
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
