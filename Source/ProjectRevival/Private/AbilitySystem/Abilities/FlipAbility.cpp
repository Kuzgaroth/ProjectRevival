// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/FlipAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/AbilityTasks/FlipTask_FlipToggle.h"

UFlipAbility::UFlipAbility()
{
}

void UFlipAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	//далее идет место с твоей логикой, туда ты вставляешь свой код
	//--------------------------------------------------------------------
	const auto Owner = ActorInfo->OwnerActor.Get();
	if (!Owner)
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Unable to get Owner Actor"))
		K2_EndAbility();
	}
	FlipTask = UFlipTask_FlipToggle::FlipInit(this, FlipCurve);
	FlipTask->OnFlipStarted.BindUFunction(this, "FlipStarted");
	FlipTask->OnFlipFinished.BindUFunction(this, "FlipFinished");
	DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, FlipDuration);
	DelayTask->OnFinish.AddDynamic(this, &UFlipAbility::OnFlipBegin);
	FlipTask->Activate(FlipStrength, FlipDuration, FlipCurve);
	//--------------------------------------------------------------------
	K2_EndAbility();
}

void UFlipAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFlipAbility::OnFlipBegin()
{
	FlipTask->OnFlipStarted.Unbind();
	DelayTask->Activate();	
}

void UFlipAbility::OnFlipEnd()
{
	FlipTask->OnFlipFinished.Unbind();
	FlipTask->EndTask();
	K2_EndAbility();
}


