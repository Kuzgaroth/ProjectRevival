// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/GrenadeAbility.h"
#include "AbilitySystemComponent.h"
#include "SoldierEnemy.h"
#include "Interfaces/IGrenadeThrower.h"

UGrenadeAbility::UGrenadeAbility()
{
	AbilityAction = EGASInputActions::GrenadeThrow;
}

void UGrenadeAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								  const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->OwnerActor->GetClass()->ImplementsInterface(UIGrenadeThrower::StaticClass()))
	{
		//TODO finish inner switching of a grenade
		TSubclassOf<ABaseGrenade> TmpGrenade = Cast<IIGrenadeThrower>(ActorInfo->OwnerActor.Get())->GetCurrentGrenade();
		if (TmpGrenade->GetClass() != CurrentGrenade->GetClass())
		{
			CurrentGrenade = TmpGrenade;
		}
	}
	GrenadeTask = UGrenadeTask_ThrowGrenade::ThrowGrenade(this, CurrentGrenade, ThrowForce, GrenadeSocketName);
	GrenadeTask->Activate();
	GrenadeTask->EndTask();
	K2_EndAbility();
}

void UGrenadeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
