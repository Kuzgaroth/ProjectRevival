#include "AbilitySystem/Abilities/VampireAbility.h"
#include "AbilitySystem/AbilityTasks/VampireAbility_TraceTask.h"

UVampireAbility::UVampireAbility()
{
	AbilityAction = EGASInputActions::Vampire;
}

void UVampireAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	//далее идет место с твоей логикой, туда ты вставляешь свой код
	//--------------------------------------------------------------------
	const auto Owner = ActorInfo->OwnerActor.Get();
	if (!Owner)
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Unable to get Owner Actor"));
		K2_EndAbility();
	}
	else
	{
		const APlayerCharacter* Character = Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get());
		//const APlayerController* Controller = Cast<APlayerController>(Character->GetController());
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if(VampireMontage != nullptr && AnimInstance != nullptr)
		{
			MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None,
				VampireMontage, true, NAME_None, true);
			MontageTask->ReadyForActivation();
		}
		else
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("No montage founded"));
		}
        //DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, FlipDuration);
        //DelayTask->OnFinish.AddDynamic(this, &UFlipAbility::OnFlipEnd);
        //TraceTask->Activate();
		//DelayTask->ReadyForActivation();
	}
	//--------------------------------------------------------------------
	//K2_EndAbility();
}


void UVampireAbility::OnTraceAnalysisEnd()
{
	//TraceTask->FlipFinished();
	//TraceTask->EndTask();
	K2_EndAbility();
}

void UVampireAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
