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
	UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Name of a CurrentGrenade: %s"), *Cast<IIGrenadeThrower>(ActorInfo->OwnerActor.Get())->GetCurrentGrenade()->GetName());
	if (ActorInfo->OwnerActor->GetClass()->ImplementsInterface(UIGrenadeThrower::StaticClass()))
	{
		//TODO finish inner switching of a grenade
		TSubclassOf<ABaseGrenade> TmpGrenade = Cast<IIGrenadeThrower>(ActorInfo->OwnerActor.Get())->GetCurrentGrenade();
		// UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Name of a Tmpgrenade via: %s"), *TmpGrenade->GetDefaultObjectName().ToString());
		// UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Name of a CurrentGrenade via: %s"), *CurrentGrenade->GetDefaultObjectName().ToString());
		if (TmpGrenade->GetDefaultObjectName() != CurrentGrenade->GetDefaultObjectName())
		{
			CurrentGrenade = TmpGrenade;
		}
		UE_LOG(LogPRAbilitySystemBase, Log, TEXT("THIS FUCKING SPAWNED GRENADE IS: %s"), *CurrentGrenade->GetName());
	}
	// UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Speed passed in Ability: %f"), ThrowForce);
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
