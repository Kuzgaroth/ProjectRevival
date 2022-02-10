// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/MeleeAttackAbility.h"
#include "GameplayTask.h"

UMeleeAttackAbility::UMeleeAttackAbility()
{
	AbilityAction = EGASInputActions::MeleeAttack;
}

void UMeleeAttackAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	const auto Owner = ActorInfo->OwnerActor.Get();
	if (!Owner)
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Unable to get Owner Actor"))
		K2_EndAbility();
	}
	
	const int MontageIndex = rand() % 3;
	switch(MontageIndex)
	{
	case 0:
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("AttackMontage1"));
		AttackMontage = AttackMontage1;
		break;
	case 1:
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("AttackMontage2"));
		AttackMontage = AttackMontage2;
		break;
	case 2:
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("AttackMontage3"));
		AttackMontage = AttackMontage3;
		break;
	default:
		break;
	}
	UE_LOG(LogPRAbilitySystemBase, Error, TEXT("!!!"));
	AttackTask = UMeleeAttackTask_Hit::AttackInit(this, AttackCurve, AttackMontage);
	AttackTask->OnAttackStarted.BindUFunction(this, "AttackStarted");
	AttackTask->OnAttackFinished.BindUFunction(this, "AttackFinished");
	DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, AttackDuration);
	DelayTask->OnFinish.AddDynamic(this, &UMeleeAttackAbility::OnAttackBegin);
	UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("AttackTask->Activate();"));	
	AttackTask->Activate();
}

void UMeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMeleeAttackAbility::OnAttackBegin()
{
	AttackTask->OnAttackStarted.Unbind();
	DelayTask->Activate();	
}

void UMeleeAttackAbility::OnAttackEnd()
{
	AttackTask->OnAttackFinished.Unbind();
	AttackTask->EndTask();
	K2_EndAbility();
}