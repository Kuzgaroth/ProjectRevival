// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/MeleeAttackAbility.h"
#include "AssassinEnemy.h"
#include "GameplayTask.h"
#include "WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	AttackTask = UMeleeAttackTask_Hit::AttackInit(this, AttackCurve);
	AttackTask->OnAttackStarted.BindUFunction(this, "AttackStarted");
	AttackTask->OnAttackFinished.BindUFunction(this, "AttackFinished");
	DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, AttackDuration);
	DelayTask->OnFinish.AddDynamic(this, &UMeleeAttackAbility::OnAttackBegin);
	
	const int MontageIndex = rand() % 3;
	switch(MontageIndex)
	{
	case 0:
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("AttackMontage1"));
		AttackTask->Activate(AttackDuration, AttackCurve, AttackMontage1);
		break;
	case 1:
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("AttackMontage2"));
		AttackTask->Activate(AttackDuration, AttackCurve, AttackMontage2);
		break;
	case 2:
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("AttackMontage3"));
		AttackTask->Activate(AttackDuration, AttackCurve, AttackMontage3);
		break;
	}
	K2_EndAbility();
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
	//AActor* OwningActor = GetOwningActorFromActorInfo();
	//AAssassinEnemy* OwningCharacter = Cast<AAssassinEnemy>(OwningActor);
	//OwningCharacter->GetMovementComponent()->
	AttackTask->OnAttackFinished.Unbind();
	AttackTask->EndTask();
	K2_EndAbility();
}