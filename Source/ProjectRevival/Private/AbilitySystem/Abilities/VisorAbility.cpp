// Project Revival. All Rights Reserved


// ReSharper disable CppTooWideScope
#include "AbilitySystem/Abilities/VisorAbility.h"
#include "BaseCharacter.h"

UVisorAbility::UVisorAbility()
{
}

void UVisorAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								  const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	UE_LOG(LogPRAbilitySystemBase, Display, TEXT("%s has started"), *GetName());
	if (UseVisorAnim)
	{
		this->SetCurrentMontage(UseVisorAnim);
		UAnimInstance* AnimInstance = this->CurrentActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			if (this->GetAbilitySystemComponentFromActorInfo()->PlayMontage(this, this->GetCurrentActivationInfo(), UseVisorAnim, 1.0f))
			{
				VisorTask = UVisorTask_SpawnVisor::SpawnVisor(this, VisorRadius, VisorDuration, ObjectTypesToVisor, ParticleEffect);
				VisorTask->Activate();
				VisorTask->EndTask();
				K2_EndAbility();
			}
		}
	}
	else
	{
		VisorTask = UVisorTask_SpawnVisor::SpawnVisor(this, VisorRadius, VisorDuration, ObjectTypesToVisor, ParticleEffect);
		VisorTask->Activate();
		VisorTask->EndTask();
		K2_EndAbility();
	}
	
}

void UVisorAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
