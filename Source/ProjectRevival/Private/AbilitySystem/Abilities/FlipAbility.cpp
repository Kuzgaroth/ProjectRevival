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
		if(!GetDirectionalMontage(Character, Controller)) K2_EndAbility();
		FlipDuration = FlipMontage->GetPlayLength() - 0.5f;
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
	FVector MovementInputDirection;
	if(Character)
	{
		//if(Character->GetCharacterMovement()->Velocity.Size() > 300.f || (Character->GetCharacterMovement()->Velocity.Size() == 0.f))
		//if(Character->GetCharacterMovement()->Velocity.Size() > 300.f)
		//{
		//	FlipMontage = ForwardMontage;
		//	FlipDirection = Character->GetActorForwardVector();
		//	return true;
		//}
		//else
		{
			if(Controller->IsInputKeyDown("W"))
			{
				if(Controller->IsInputKeyDown("A"))
				{
					UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press W+A"));
					FlipMontage = ForwardLeftMontage;
					MovementInputDirection.X = 1;
					MovementInputDirection.Y = -1;
				}
				else
				{
					if(Controller->IsInputKeyDown("D"))
					{
						UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press W+D"));
						FlipMontage = ForwardRightMontage;
						MovementInputDirection.X = 1;
						MovementInputDirection.Y = 1;
					}
					else
					{
						UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press W"));
						FlipMontage = ForwardMontage;
						MovementInputDirection.X = 1;
						MovementInputDirection.Y = 0;
					}
				}
			}
			else
			{
				if(Controller->IsInputKeyDown("S"))
				{
					if(Controller->IsInputKeyDown("A"))
					{
						UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press S+A"));
						FlipMontage = BackwardLeftMontage;
						MovementInputDirection.X = -1;
						MovementInputDirection.Y = -1;
					}
					else
					{
						if(Controller->IsInputKeyDown("D"))
						{
							UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press S+D"));
							FlipMontage = BackwardRightMontage;
							MovementInputDirection.X = -1;
							MovementInputDirection.Y = 1;
						}
						else
						{
							UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press S"));
							FlipMontage = BackwardMontage;
							MovementInputDirection.X = -1;
							MovementInputDirection.Y = 0;
						}
					}
				}
				else
				{
					if(Controller->IsInputKeyDown("A"))
					{
						UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press A"));
						FlipMontage = LeftMontage;
						MovementInputDirection.X = 0;
						MovementInputDirection.Y = -1;
					}
					else
					{
						if(Controller->IsInputKeyDown("D"))
						{
							UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("press D"));
							FlipMontage = RightMontage;
							MovementInputDirection.X = 0;
							MovementInputDirection.Y = 1;
						}
						else
						{
							FlipMontage = ForwardMontage;
							FlipDirection = Character->GetActorForwardVector();
							return true;
						}
					}
				}
			}
			FlipDirection = GetFlipDirection(Character, MovementInputDirection);
			return true;
		}
	}
	else
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Fltp Ability: Input error"));
		return false; 
	}
}


FVector UFlipAbility::GetFlipDirection(const APlayerCharacter* Character, const FVector MovementInputDirection)
{
	// Get player forward and right
	FRotator PlayerRotZeroPitch = Character->GetActorRotation();
	PlayerRotZeroPitch.Pitch = 0;
    const FVector PlayerRight = FRotationMatrix(PlayerRotZeroPitch).GetUnitAxis(EAxis::Y);
	const FVector PlayerForward = FRotationMatrix(PlayerRotZeroPitch).GetUnitAxis(EAxis::X);
	// Scale the forward and right vectors by movementInputDirection
	const FVector Direction = PlayerForward * MovementInputDirection.X + PlayerRight * MovementInputDirection.Y;
	return Direction;
}

void UFlipAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
