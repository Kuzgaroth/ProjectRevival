// Project Revival. All Rights Reserved

#include "AbilitySystem/Abilities/VisorSwitcherAbility.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.h"

UVisorSwitcherAbility::UVisorSwitcherAbility()
{
	AbilityAction = EGASInputActions::Visor;
}

void UVisorSwitcherAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								  const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	PlaySound(StartSound);
	if(Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get()))
	{
		VisorRadius = Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get())->HighlightRadius;
		VisorDuration = Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get())->DestroyDelay;
	}
	// if (UseVisorAnim)
	// {
	// 	this->SetCurrentMontage(UseVisorAnim);
	// 	UAnimInstance* AnimInstance = this->CurrentActorInfo->GetAnimInstance();
	// 	if (AnimInstance != nullptr)
	// 	{
	// 		if (this->GetAbilitySystemComponentFromActorInfo()->PlayMontage(this, this->GetCurrentActivationInfo(), UseVisorAnim, 1.0f))
	// 		{
	// 			VisorTaskSwitcher = UVisorSwitcherTask_TriggerVisor::SpawnVisor(this, VisorRadius, VisorDuration, ObjectTypesToVisor, ParticleEffect, OnMaterial);
	// 			VisorTaskSwitcher->Activate();
	// 		}
	// 	}
	// }
	// else
	// {
	// 	VisorTaskSwitcher = UVisorSwitcherTask_TriggerVisor::SpawnVisor(this, VisorRadius, VisorDuration, ObjectTypesToVisor, ParticleEffect, OnMaterial);
	// 	VisorTaskSwitcher->Activate();
	// }
	VisorTaskSwitcher = UVisorSwitcherTask_TriggerVisor::SpawnVisor(this, VisorRadius, VisorDuration, ObjectTypesToVisor, ParticleEffect, OnMaterial);
	VisorTaskSwitcher->Activate();
	if(Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get()))
	{
		Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get())->VisorPressedDelegate.Broadcast();
	}
}

void UVisorSwitcherAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		if(Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get()))
		{
			Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get())->VisorReleasedDelegate.Broadcast();
		}
		if (VisorTaskSwitcher) VisorTaskSwitcher->EndTask();
		K2_EndAbility();
	}
}

void UVisorSwitcherAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PlaySound(EndSound);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
