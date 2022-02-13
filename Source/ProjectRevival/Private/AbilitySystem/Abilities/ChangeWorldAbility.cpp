// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/ChangeWorldAbility.h"

void UChangeWorldAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	const auto Owner = ActorInfo->OwnerActor.Get();
	if (!Owner)
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Unable to get Owner Actor"))
		K2_EndAbility();
	}
	ChangeWorldTask=UChangeWorldTask_SpawnSphere::ChangeWorldInit(this,ChangeWorldShere,TraceSpawnDistance);
	ChangeWorldTask->Activate(*ActorInfo->OwnerActor);
	ChangeWorldTask->EndTask();
	K2_EndAbility();
	
	
}

void UChangeWorldAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

