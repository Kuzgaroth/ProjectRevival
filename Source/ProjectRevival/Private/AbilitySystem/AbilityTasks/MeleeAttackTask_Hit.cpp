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
	UCurveFloat* AttackCurve, UAnimMontage* Montage)
{
	const auto AbilityTask = NewAbilityTask<UMeleeAttackTask_Hit>(OwningAbility, FName("AttackTask"));
	AbilityTask->CurveFloat = AttackCurve;
	AbilityTask->bTickingTask = 1;
	AbilityTask->MeleeAttackMontage = Montage;
	return AbilityTask;
}

void UMeleeAttackTask_Hit::Activate()
{
	Super::Activate();
	UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Task activated"));
	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
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
			MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, NAME_None, MeleeAttackMontage,
				PlayRate, NAME_None, true, 1.f, 0.f);
			Timeline.SetTimelineFinishedFunc(OnAttackStarted);
			Timeline.PlayFromStart();
			MontageTask->OnBlendOut.AddDynamic(this, &UMeleeAttackTask_Hit::AttackFinished);
			MontageTask->ReadyForActivation();
		}
		else
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("No montage founded"));
		}
		AttackFinished();
	}
}

void UMeleeAttackTask_Hit::AttackFinished()
{
	AAssassinEnemy* Character = Cast<AAssassinEnemy>(GetAvatarActor());
	const UWeaponComponent* WeaponComponent = Cast<UWeaponComponent>(Character->GetWeaponComponent());
	AMeleeWeapon* Weapon = Cast<AMeleeWeapon>(WeaponComponent->GetCurrentWeapon());
	
	if(Weapon->IsHitDone())
	{
		//DO DAMAGE
		Weapon->ResetHitStatus();
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Damage done!"));
	}
	Weapon->ToggleCollisionOff();
	Character->SetIsAttacking(false);
	
	Timeline.SetTimelineFinishedFunc(OnAttackFinished);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	MontageTask->EndTask();
	this->EndTask();
}

void UMeleeAttackTask_Hit::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}