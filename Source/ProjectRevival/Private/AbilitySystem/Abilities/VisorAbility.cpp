// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/VisorAbility.h"

UVisorAbility::UVisorAbility()
{
}

void UVisorAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	//далее идет место с твоей логикой, туда ты вставляешь свой код
    //--------------------------------------------------------------------

	//--------------------------------------------------------------------
	K2_EndAbility();
	
}

void UVisorAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	
}
