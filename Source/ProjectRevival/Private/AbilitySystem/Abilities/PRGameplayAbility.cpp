// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/PRGameplayAbility.h"

UPRGameplayAbility::UPRGameplayAbility()
{
	
}

void UPRGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	UE_LOG(LogPRAbilitySystemBase, Display, TEXT("%s has started"), *GetName());
	float CooldownMagnitude;
	UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect(); 
	if (CooldownEffect)
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1.f, CooldownMagnitude);
		UE_LOG(LogPRAbilitySystemBase, Display, TEXT("Cooldown is %f seconds"), CooldownMagnitude);
	}
	//K2_EndAbility();
}

void UPRGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogPRAbilitySystemBase, Display, TEXT("%s has ended"), *GetName());
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}