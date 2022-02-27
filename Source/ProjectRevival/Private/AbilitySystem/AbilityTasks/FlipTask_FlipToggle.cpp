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


UFlipTask_FlipToggle* UFlipTask_FlipToggle::FlipInit(UGameplayAbility* OwningAbility, UCurveFloat* FlipCurve,
	float Strength, float Duration, UAnimMontage* Montage, FVector Direction)
{
	const auto AbilityTask = NewAbilityTask<UFlipTask_FlipToggle>(OwningAbility, FName("FlipTask"));
	AbilityTask->CurveFloat = FlipCurve;
	AbilityTask->bTickingTask = 1;
	AbilityTask->FlipDuration = Duration;
	AbilityTask->FlipStrength = Strength;
	AbilityTask->FlipMontage = Montage;
	AbilityTask->FlipDirection = Direction;
	return AbilityTask;
}

void UFlipTask_FlipToggle::Activate()
{
	Super::Activate();
	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		Timeline.AddInterpFloat(CurveFloat,TimeLineProgress);
		Timeline.SetLooping(false);
		FlipStarted();
	}
}

void UFlipTask_FlipToggle::TickTimeline(float Delta)
{
	if (Timeline.IsPlaying()) Timeline.TickTimeline(Delta);
}

void UFlipTask_FlipToggle::FlipStarted()
{
	APlayerCharacter* const Character = Cast<APlayerCharacter>(GetOwnerActor());
	UWeaponComponent* Weapon = Cast<UWeaponComponent>(Character->GetWeaponComponent());
	
	if(Character->GetCharacterMovement()->IsFlying()||Character->GetCharacterMovement()->IsFalling()
		||!Weapon->CanFire())
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Flip failed"));
		EndTask();
	}
	else
	{
		APlayerController* Controller = Cast<APlayerController>(Character->GetController());
		
		Timeline.SetTimelineFinishedFunc(OnFlipStarted);
		Timeline.PlayFromStart();

		Character->CameraZoomOut();
		Character->DisableInput(Controller);
		Character->bIsFlipping = true;
		if(Weapon->IsShooting())
		{
			Weapon->StopFire();
		}
		ForceTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			Ability,
			NAME_None,
			FlipDirection,
			FlipStrength,
			FlipDuration,
			false,
			CurveFloat,
			ERootMotionFinishVelocityMode::SetVelocity,
			Character->GetCharacterMovement()->Velocity,
			0.f,
			false);
		
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if(FlipMontage != nullptr && AnimInstance != nullptr)
		{
			MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, NAME_None,
				FlipMontage, true, NAME_None, true);
			MontageTask->ReadyForActivation();
		}
		else
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("No montage founded"));
		}
	}
}

void UFlipTask_FlipToggle::FlipFinished()
{
	Timeline.SetTimelineFinishedFunc(OnFlipFinished);
	
	ABaseCharacter* const Character = Cast<ABaseCharacter>(GetOwnerActor());
	APlayerController* Controller = Cast<APlayerController>(Character->GetController());
	Character->EnableInput(Controller);
	Character->bIsFlipping = false;
	
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UFlipTask_FlipToggle::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}