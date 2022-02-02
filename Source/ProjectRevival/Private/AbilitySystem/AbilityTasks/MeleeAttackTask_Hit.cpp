// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/MeleeAttackTask_Hit.h"
#include "AssassinEnemy.h"
#include "AbilitySystem/PRAbilityTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Player/BaseCharacter.h"


void UMeleeAttackTask_Hit::Activate()
{
}

void UMeleeAttackTask_Hit::Activate(float Duration, UCurveFloat* Curve, FName MeleeAttackMontage)
{
	Super::Activate();
	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		Timeline.AddInterpFloat(CurveFloat,TimeLineProgress);
		Timeline.SetLooping(false);
		AttackStarted(Duration, Curve, MeleeAttackMontage);
	}
}

UMeleeAttackTask_Hit* UMeleeAttackTask_Hit::AttackInit(UGameplayAbility* OwningAbility,
	UCurveFloat* AttackCurve)
{
	const auto AbilityTask = NewAbilityTask<UMeleeAttackTask_Hit>(OwningAbility, FName("AttackTask"));
	AbilityTask->CurveFloat = AttackCurve;
	AbilityTask->bTickingTask = 1;
	return AbilityTask;
}

void UMeleeAttackTask_Hit::TickTimeline(float Delta)
{
	if (Timeline.IsPlaying()) Timeline.TickTimeline(Delta);
}

void UMeleeAttackTask_Hit::AttackStarted(float Duration, UCurveFloat* Curve, FName MeleeAttackMontage)
{
	AAssassinEnemy* const Character = Cast<AAssassinEnemy>(GetAvatarActor());
	if(Character->GetCharacterMovement()->IsFlying()||Character->GetCharacterMovement()->IsFalling()
		||Character->IsAttacking())
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Melee attack failed"));
		EndTask();
	}
	else
	{
		UE_LOG(LogPRAbilitySystemBase, Display, TEXT("Melee attack has started"));
		Timeline.SetTimelineFinishedFunc(OnAttackStarted);
		Timeline.PlayFromStart();
		
		int MontageIndex = rand() % 3;
		switch(MontageIndex)
		{
		case 0:
			AttackMontage = AttackMontage1;
		case 1:
			AttackMontage = AttackMontage2;
		case 2:
			AttackMontage = AttackMontage3;
		}
		
		if(AttackMontage != nullptr)
		{	
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if(AnimInstance != nullptr)
			{
				MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, MeleeAttackMontage, AttackMontage, true, NAME_None, true);
			}
		}
		else
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("No montage founded"));
		}
		
		//if(CurrentWeapon == nullptr)
		//	return;
	
		/*	UStaticMeshComponent * WeaponMesh = CurrentWeapon->GetMesh();
	 
			FVector TraceStart = WeaponMesh->GetSocketLocation("TraceStart");
			FVector TraceEnd = WeaponMesh->GetSocketLocation("TraceEnd");
	 
			if(GetWorld()->LineTrace(TraceStart, TraceEnd, HitResult))
			{
				if(HitResult.Actor->IsA<ACharacter>())
					UGameplayStatics::ApplyPointDamage(Damage, HitResult.Actor);
			}*/
		AttackFinished();
	}
}

void UMeleeAttackTask_Hit::AttackFinished()
{
	Timeline.SetTimelineFinishedFunc(OnAttackFinished);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UMeleeAttackTask_Hit::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}