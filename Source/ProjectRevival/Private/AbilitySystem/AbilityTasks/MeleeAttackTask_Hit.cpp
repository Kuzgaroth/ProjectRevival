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
	UCurveFloat* AttackCurve, UAnimMontage* Montage, float PlayRate, float AttackDuration)
{
	const auto AbilityTask = NewAbilityTask<UMeleeAttackTask_Hit>(OwningAbility, FName("AttackTask"));
	AbilityTask->CurveFloat = AttackCurve;
	AbilityTask->bTickingTask = 1;
	AbilityTask->MeleeAttackMontage = Montage;
	AbilityTask->PlayRate = PlayRate;
	AbilityTask->AttackDuration = AttackDuration;
	return AbilityTask;
}

void UMeleeAttackTask_Hit::Activate()
{
	Super::Activate();
	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		Timeline.AddInterpFloat(CurveFloat,TimeLineProgress);
		Timeline.SetLooping(false);
		
		const AAssassinEnemy* Character = Cast<AAssassinEnemy>(GetAvatarActor());
		UWeaponComponent* WeaponComponent = Cast<UWeaponComponent>(Character->GetWeaponComponent());
		AMeleeWeapon* Weapon = Cast<AMeleeWeapon>(WeaponComponent->GetCurrentWeapon());
	
		MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, NAME_None, MeleeAttackMontage,
			PlayRate, NAME_None, true, 1.f, 0.f);
		MontageTask->ReadyForActivation();
		Weapon->ToggleCollisionOn();
		Weapon->AddNewBeam();
		Character->GetMovementComponent()->SetJumpAllowed(false);
	}
	else
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Curve not found"));
	}
}

void UMeleeAttackTask_Hit::AttackFinished()
{
	const AAssassinEnemy* Character = Cast<AAssassinEnemy>(GetAvatarActor());
	UWeaponComponent* WeaponComponent = Cast<UWeaponComponent>(Character->GetWeaponComponent());
	AMeleeWeapon* Weapon = Cast<AMeleeWeapon>(WeaponComponent->GetCurrentWeapon());
	
	Weapon->ResetHitStatus();
	Character->GetMovementComponent()->SetJumpAllowed(true);
	Weapon->ToggleCollisionOff();
	
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	MontageTask->EndTask();
	this->EndTask();
}

void UMeleeAttackTask_Hit::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}