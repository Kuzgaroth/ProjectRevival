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
		
	AAssassinEnemy* Character = Cast<AAssassinEnemy>(ActorInfo->OwnerActor.Get());
	const UWeaponComponent* WeaponComponent = Cast<UWeaponComponent>(Character->GetWeaponComponent());
	AMeleeWeapon* Weapon = Cast<AMeleeWeapon>(WeaponComponent->GetCurrentWeapon());
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	
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
	
	if(Weapon && Character && !Character->GetMovementComponent()->IsFalling() &&
		!Character->GetMovementComponent()->IsFlying() && !Character->IsAttacking() &&
		AttackMontage != nullptr && AnimInstance != nullptr)
	{
		AttackDuration = AttackMontage->GetPlayLength();
		AttackTask = UMeleeAttackTask_Hit::AttackInit(this, AttackCurve, AttackMontage, PlayRate, AttackDuration);
		DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, AttackDuration / PlayRate);
		DelayTask->OnFinish.AddDynamic(this, &UMeleeAttackAbility::OnDelayEnd);
		AttackTask->Activate();
		DelayTask->ReadyForActivation();
	}
	else
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Melee attack failed"));
		K2_EndAbility();
	}
}

void UMeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMeleeAttackAbility::OnDelayEnd()
{
	AttackTask->AttackFinished();
	K2_EndAbility();
}
