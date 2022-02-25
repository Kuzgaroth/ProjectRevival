// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/FlipAbility.h"
#include "AbilitySystem/AbilityTasks/FlipTask_FlipToggle.h"
#include "GameFramework/PlayerInput.h"

UFlipAbility::UFlipAbility()
{
	AbilityAction = EGASInputActions::Flip;
}

void UFlipAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	//далее идет место с твоей логикой, туда ты вставляешь свой код
	//--------------------------------------------------------------------
	const auto Owner = ActorInfo->OwnerActor.Get();
	if (!Owner)
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Unable to get Owner Actor"));
		K2_EndAbility();
	}
	else
	{
		const APlayerCharacter* Character = Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get());
		const APlayerController* Controller = Cast<APlayerController>(Character->GetController());
		if(!GetDirectionalMontage(Character, Controller)) K2_EndAbility();;
		//FlipDirection = Character->GetActorForwardVector();
		//FlipMontage = ForwardMontage;
		FlipTask = UFlipTask_FlipToggle::FlipInit(this, FlipCurve, FlipStrength, FlipDuration,
			FlipMontage, FlipDirection);
        DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, FlipDuration);
        DelayTask->OnFinish.AddDynamic(this, &UFlipAbility::OnFlipEnd);
        FlipTask->Activate();
		DelayTask->ReadyForActivation();
	}
	//--------------------------------------------------------------------
	//K2_EndAbility();
}

bool UFlipAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* GameplayTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool SuperResult = Super::CanActivateAbility(Handle, ActorInfo, GameplayTags, TargetTags, OptionalRelevantTags);
	if (Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get())->GetCoverData().IsInCover()) return false;
	return SuperResult;
}

void UFlipAbility::OnFlipEnd()
{
	FlipTask->FlipFinished();
	FlipTask->EndTask();
	K2_EndAbility();
}

bool UFlipAbility::GetDirectionalMontage(const APlayerCharacter* Character, const APlayerController* Controller)
{
	if(Controller->IsInputKeyDown("W") || (Character->GetCharacterMovement()->Velocity.Size() == 0.f))
	{
		if(Controller->IsInputKeyDown("A"))
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press W+A"));
			FlipMontage = ForwardLeftMontage;
			
			FlipDirection = Character->GetActorForwardVector() - Character->GetActorRightVector();
			return true;
		}
		if(Controller->IsInputKeyDown("D"))
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press W+D"));
			FlipMontage = ForwardRightMontage;
			FlipDirection = Character->GetActorForwardVector() + Character->GetActorRightVector();
			return true;
		}
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press W"));
		FlipMontage = ForwardMontage;
		FlipDirection = Character->GetActorForwardVector();
		return true;
	}
	if(Controller->IsInputKeyDown("S"))
	{
		if(Controller->IsInputKeyDown("A"))
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press S+A"));
			FlipMontage = BackwardLeftMontage;
			FlipDirection = -Character->GetActorForwardVector() - Character->GetActorRightVector();
			return true;
		}
		if(Controller->IsInputKeyDown("D"))
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press S+D"));
			FlipMontage = BackwardRightMontage;
			FlipDirection = -Character->GetActorForwardVector() + Character->GetActorRightVector();
			return true;
		}
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press S"));
		FlipMontage = BackwardMontage;
		FlipDirection = -Character->GetActorForwardVector();
		return true;
	}
	if(Controller->IsInputKeyDown("A"))
	{
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press A"));
		FlipMontage = LeftMontage;
		FlipDirection = -Character->GetActorRightVector();
		return true;
	}
	if(Controller->IsInputKeyDown("D"))
	{
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press D"));
		FlipMontage = RightMontage;
		FlipDirection = Character->GetActorRightVector();
		return true;
	}
	return false;
}

void UFlipAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
