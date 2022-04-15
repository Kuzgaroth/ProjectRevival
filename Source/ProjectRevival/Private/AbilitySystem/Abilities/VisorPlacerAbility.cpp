// Project Revival. All Rights Reserved

#include "AbilitySystem/Abilities/VisorPlacerAbility.h"
#include "BaseCharacter.h"

UVisorPlacerAbility::UVisorPlacerAbility()
{
	AbilityAction = EGASInputActions::Visor;
}

void UVisorPlacerAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								  const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	if (UseVisorAnim)
	{
		this->SetCurrentMontage(UseVisorAnim);
		UAnimInstance* AnimInstance = this->CurrentActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			if (this->GetAbilitySystemComponentFromActorInfo()->PlayMontage(this, this->GetCurrentActivationInfo(), UseVisorAnim, 1.0f))
			{
				VisorTask = UVisorPlacerTask_SpawnVisor::SpawnVisor(this, VisorRadius, VisorDuration, ObjectTypesToVisor, ParticleEffect);
				VisorTask->Activate();
				VisorTask->EndTask();
				K2_EndAbility();
			}
		}
	}
	else
	{
		VisorTask = UVisorPlacerTask_SpawnVisor::SpawnVisor(this, VisorRadius, VisorDuration, ObjectTypesToVisor, ParticleEffect);
		VisorTask->Activate();
		VisorTask->EndTask();
		K2_EndAbility();
	}
	
}

void UVisorPlacerAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
