// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/FlipTask_FlipToggle.h"
#include "AbilitySystem/PRAbilityTypes.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Player/BaseCharacter.h"


void UFlipTask_FlipToggle::Activate()
{
}

void UFlipTask_FlipToggle::Activate(float Strength, float Duration, UCurveFloat* Curve)
{
	Super::Activate();
	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		Timeline.AddInterpFloat(CurveFloat,TimeLineProgress);
		Timeline.SetLooping(false);
		FlipStarted(Strength, Duration, Curve);
	}
}

UFlipTask_FlipToggle* UFlipTask_FlipToggle::FlipInit(UGameplayAbility* OwningAbility,
	UCurveFloat* FlipCurve)
{
	const auto AbilityTask = NewAbilityTask<UFlipTask_FlipToggle>(OwningAbility, FName("FlipTask"));
	AbilityTask->CurveFloat = FlipCurve;
	AbilityTask->bTickingTask = 1;
	return AbilityTask;
}

void UFlipTask_FlipToggle::TickTimeline(float Delta)
{
	if (Timeline.IsPlaying()) Timeline.TickTimeline(Delta);
}

void UFlipTask_FlipToggle::FlipStarted(float Strength, float Duration, UCurveFloat* Curve)
{
	const FGameplayTag FlipTag = FGameplayTag::RequestGameplayTag(FName("Ability.Flip.IsFlipping"));
	APlayerCharacter* const Character = Cast<APlayerCharacter>(GetAvatarActor());
		UWeaponComponent* Weapon = Cast<UWeaponComponent>(Character->GetWeaponComponent());
	if(Character->GetCharacterMovement()->IsFlying()||Character->GetCharacterMovement()->IsFalling()
		||Weapon->IsShooting()||!Weapon->CanFire())
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Flip failed"));
		EndTask();
	}
	else
	{
		UE_LOG(LogPRAbilitySystemBase, Display, TEXT("Flip has started"));
		Timeline.SetTimelineFinishedFunc(OnFlipStarted);
		Timeline.PlayFromStart();
		Character->GetPlayerSpringArmComponent()->bUsePawnControlRotation = false;
		APawn* ACharacter = Cast<APawn>(GetAvatarActor());
		ACharacter->bUseControllerRotationYaw = false;
		const FVector Forward  = Character->GetActorForwardVector();
		const FVector TempVelocity = Character->GetCharacterMovement()->Velocity;
		ForceTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			Ability,
			NAME_None,
			Forward,
			Strength,
			Duration,
			false,
			Curve,
			ERootMotionFinishVelocityMode::SetVelocity,
			TempVelocity,
			0.f,
			false);
		if(FlipMontage != nullptr)
		{	
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if(AnimInstance != nullptr)
			{
				MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, NAME_None, FlipMontage, true, NAME_None, true);
			}
		}
		else
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("No montage founded"));
		}
		FlipFinished();
	}
}

void UFlipTask_FlipToggle::FlipFinished()
{
	Timeline.SetTimelineFinishedFunc(OnFlipFinished);
	APlayerCharacter* const Character = Cast<APlayerCharacter>(GetAvatarActor());
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Character->GetPlayerSpringArmComponent()->bUsePawnControlRotation = true;
	APawn* ACharacter = Cast<APawn>(GetAvatarActor());
	ACharacter->bUseControllerRotationYaw = true;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UFlipTask_FlipToggle::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}