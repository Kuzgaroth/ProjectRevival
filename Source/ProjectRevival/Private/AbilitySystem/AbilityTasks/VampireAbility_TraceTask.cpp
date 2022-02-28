// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/VampireAbility_TraceTask.h"
#include "AbilitySystem/PRAbilityTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Player/BaseCharacter.h"


UVampireAbility_TraceTask* UVampireAbility_TraceTask::TaskInit(UGameplayAbility* OwningAbility, UCurveFloat* FlipCurve,
	float Strength, float Duration, UAnimMontage* Montage, FVector Direction)
{
	const auto AbilityTask = NewAbilityTask<UVampireAbility_TraceTask>(OwningAbility, FName("FlipTask"));
	AbilityTask->VampireMontage = Montage;
	return AbilityTask;
}

void UVampireAbility_TraceTask::Activate()
{
	Super::Activate();
	//if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		//Timeline.AddInterpFloat(CurveFloat,TimeLineProgress);
		Timeline.SetLooping(false);
		TraceAnalysisStarted();
	}
}

void UVampireAbility_TraceTask::TickTimeline(float Delta)
{
	if (Timeline.IsPlaying()) Timeline.TickTimeline(Delta);
}

void UVampireAbility_TraceTask::TraceAnalysisStarted()
{
	APlayerCharacter* const Character = Cast<APlayerCharacter>(GetOwnerActor());
	
	if(Character->GetCharacterMovement()->IsFlying()||Character->GetCharacterMovement()->IsFalling())
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("TraceAnalysisStarted failed"));
		EndTask();
	}
	else
	{
		APlayerController* Controller = Cast<APlayerController>(Character->GetController());
		
		Timeline.SetTimelineFinishedFunc(OnAnalysisStarted);
		Timeline.PlayFromStart();

		Character->CameraZoomOut();
		Character->DisableInput(Controller);
		Character->bIsFlipping = true;
				
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if(VampireMontage != nullptr && AnimInstance != nullptr)
		{
			MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, NAME_None,
				VampireMontage, true, NAME_None, true);
			MontageTask->ReadyForActivation();
			//MontageTask->OnCompleted.AddDynamic(this, &UVampireAbility::OnFlipEnd);
		}
		else
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("No montage founded"));
		}
	}
}

void UVampireAbility_TraceTask::TraceAnalysisFinished()
{
	Timeline.SetTimelineFinishedFunc(OnAnalysisFinished);
	
	ABaseCharacter* const Character = Cast<ABaseCharacter>(GetOwnerActor());
	APlayerController* Controller = Cast<APlayerController>(Character->GetController());
	Character->EnableInput(Controller);
	Character->bIsFlipping = false;
	
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UVampireAbility_TraceTask::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}
