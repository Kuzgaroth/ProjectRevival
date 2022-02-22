// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/MeleeAttackTask_Hit.h"
#include "AssassinEnemy.h"
#include "AbilitySystem/PRAbilityTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseCharacter.h"
#include "WeaponComponent.h"

void UMeleeAttackTask_Hit::TickTimeline(float Delta)
{
	if (Timeline.IsPlaying()) Timeline.TickTimeline(Delta);
}

UMeleeAttackTask_Hit* UMeleeAttackTask_Hit::AttackInit(UGameplayAbility* OwningAbility,
	UCurveFloat* AttackCurve, UAnimMontage* Montage, float PlayRate)
{
	const auto AbilityTask = NewAbilityTask<UMeleeAttackTask_Hit>(OwningAbility, FName("AttackTask"));
	AbilityTask->CurveFloat = AttackCurve;
	AbilityTask->bTickingTask = 1;
	AbilityTask->MeleeAttackMontage = Montage;
	AbilityTask->PlayRate = PlayRate;
	return AbilityTask;
}

void UMeleeAttackTask_Hit::Activate()
{
	Super::Activate();
	UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Task activated"));
	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("TimeLineProgress"));
		TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		Timeline.AddInterpFloat(CurveFloat,TimeLineProgress);
		Timeline.SetLooping(false);
		AttackStarted();
	}
	else
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Curve not found"));
	}
}

void UMeleeAttackTask_Hit::AttackStarted()
{
	AAssassinEnemy* Character = Cast<AAssassinEnemy>(GetAvatarActor());
	const UWeaponComponent* WeaponComponent = Cast<UWeaponComponent>(Character->GetWeaponComponent());
	AMeleeWeapon* Weapon = Cast<AMeleeWeapon>(WeaponComponent->GetCurrentWeapon());
	
	if(!Weapon||!Character||Character->GetCharacterMovement()->IsFlying()
		||Character->GetCharacterMovement()->IsFalling()||Character->IsAttacking())
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Melee attack failed"));
		EndTask();
	}
	else
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if(MeleeAttackMontage != nullptr && AnimInstance != nullptr)
		{	
			Character->SetIsAttacking(true);
			Weapon->ToggleCollisionOn();
			MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, NAME_None, MeleeAttackMontage,
				PlayRate, NAME_None, true, 1.f, 0.f);
			MontageTask->ReadyForActivation();
			MontageTask->OnBlendOut.AddDynamic(this, &UMeleeAttackTask_Hit::CheckOverlap);
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("check hit"));
			if(Weapon->IsHitDone())
			{
				//DO DAMAGE
				Weapon->ResetHitStatus();
				UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Damage done!"));
			}
			Weapon->ToggleCollisionOff();
			Character->SetIsAttacking(false);
		}
		else
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("No montage founded"));
		}
	}
}

void UMeleeAttackTask_Hit::CheckOverlap()
{
	
}

void UMeleeAttackTask_Hit::AttackFinished()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	MontageTask->EndTask();
	this->EndTask();
}

void UMeleeAttackTask_Hit::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}