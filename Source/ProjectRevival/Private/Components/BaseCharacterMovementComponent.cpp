// Project Revival. All Rights Reserved


#include "Components/BaseCharacterMovementComponent.h"

#include "AimAnimNotify.h"
#include "AnimUtils.h"
#include "JumpAnimNotify.h"
#include "TurnAnimNotify.h"
#include "Components/CapsuleComponent.h"
#include "Player/BaseCharacter.h"

DEFINE_LOG_CATEGORY(LogPlayerMovement);

void UBaseCharacterMovementComponent::PlayTurnAnimation()
{
}

void UBaseCharacterMovementComponent::JumpPressEnded()
{
	PlayerMovementLogic.JumpPressed=false;
	PlayerMovementLogic.JumpStarted=true;
	auto JumpEndedNotify = AnimUtils::FindNotifyByClass<UJumpAnimNotify>(JumpEndAnim);
	if (JumpEndedNotify)
	{
		JumpEndedNotify->OnActionPointReached.BindUObject(this, &UBaseCharacterMovementComponent::JumpProcessEnded);
	}
	CharacterOwner->Jump();
	
}

void UBaseCharacterMovementComponent::JumpProcessEnded()
{
	PlayerMovementLogic.JumpStarted=false;
	PlayerMovementLogic.JumpPressed=false;
}

void UBaseCharacterMovementComponent::TurnFinished()
{
	PlayerMovementLogic.IsTurning=false;
	UE_LOG(LogPlayerMovement, Warning, TEXT("current Yaw is %f"), CharacterOwner->GetCapsuleComponent()->GetForwardVector().Y);
}

void UBaseCharacterMovementComponent::AimStartFinished()
{
	PlayerMovementLogic.IsPivotTargeted=true;
}

void UBaseCharacterMovementComponent::AimEndFininshed()
{
	PlayerMovementLogic.IsPivotTargeted=false;
}

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ABaseCharacter* Player = Cast<ABaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed*RunModifier:MaxSpeed;
}

void UBaseCharacterMovementComponent::TurnLeft()
{
}

void UBaseCharacterMovementComponent::TurnRight()
{
	if (PlayerMovementLogic.IsTurning) return;
	auto TurnRightNotify = AnimUtils::FindNotifyByClass<UTurnAnimNotify>(TurnRightAnim);
	if (TurnRightNotify)
	{
		TurnRightNotify->OnActionPointReached.BindUObject(this, &UBaseCharacterMovementComponent::TurnFinished);
	}
	PlayerMovementLogic.IsTurning = true;
	UE_LOG(LogPlayerMovement, Warning, TEXT("current Yaw is %f"), CharacterOwner->GetCapsuleComponent()->GetForwardVector().Y);
}

void UBaseCharacterMovementComponent::TurnBackward()
{
}

void UBaseCharacterMovementComponent::Jump()
{
	UE_LOG(LogPlayerMovement, Warning, TEXT("Jump pressed = %s, jump started = %s"),
		PlayerMovementLogic.JumpPressed ? "1" : "0",PlayerMovementLogic.JumpStarted ? "1" : "0");
	
	if (PlayerMovementLogic.IsInJump()) return;
	PlayerMovementLogic.JumpPressed = true;
	auto JumpStartedNotify = AnimUtils::FindNotifyByClass<UJumpAnimNotify>(JumpAnim);
	if (JumpStartedNotify)
	{
		JumpStartedNotify->OnActionPointReached.BindUObject(this, &UBaseCharacterMovementComponent::JumpPressEnded);
	}
}

void UBaseCharacterMovementComponent::MoveRight(float Amount)
{
	if (PlayerMovementLogic.IsInJump()) return;
	if (PlayerMovementLogic.IsTurning) return;
	if ( (CharacterOwner->GetController() != nullptr) && (Amount != 0.0f) )
	{
		const FRotator Rotation = CharacterOwner->GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		CharacterOwner->AddMovementInput(Direction, Amount);
	}
}

void UBaseCharacterMovementComponent::MoveForward(float Amount)
{
	if (PlayerMovementLogic.IsInJump()) return;
	if (PlayerMovementLogic.IsTurning) return;
	if ( (CharacterOwner->GetController() != nullptr) && (Amount != 0.0f) )
	{
		const FRotator Rotation = CharacterOwner->GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		CharacterOwner->AddMovementInput(Direction, Amount);
	}
}

void UBaseCharacterMovementComponent::AimStart()
{
	/*if (PlayerMovementLogic.IsInJump()) return;
	const auto AnimInst = CharacterOwner->GetMesh()->GetAnimInstance();
	const auto AimMont = AnimInst->Montage_IsPlaying(AimMontage);
	float Seconds=0;
	const auto Notifies = AnimUtils::FindNotifiesByClass<UAimAnimNotify>(AimMontage);
	Notifies[0]->OnActionPointReached.Unbind();
	auto AnimStartNotify = Notifies[1];
	if (AnimStartNotify)
	{
		AnimStartNotify->OnActionPointReached.BindUObject(this, &UBaseCharacterMovementComponent::AimStartFinished);
	}
	if (AimMont && AimMontage)
	{
		const float PlayRate = AnimInst->Montage_GetPlayRate(AimMontage);
		Seconds = AnimInst->Montage_GetPosition(AimMontage);
		if (Seconds>0.0f && PlayRate==-1)
		{
			AnimInst->Montage_Stop(0.1, AimMontage);
			
		}
	}
	AnimInst->Montage_Play(AimMontage, 1, EMontagePlayReturnType::MontageLength,
				Seconds, true);*/
	PlayerMovementLogic.IsPivotTargeted=true;
}

void UBaseCharacterMovementComponent::AimEnd()
{
	/*const auto AnimInst = CharacterOwner->GetMesh()->GetAnimInstance();
	const auto AimMont = AnimInst->Montage_IsPlaying(AimMontage);
	float Seconds=0;
	const auto Notifies = AnimUtils::FindNotifiesByClass<UAimAnimNotify>(AimMontage);
	Notifies[1]->OnActionPointReached.Unbind();
	auto AnimStartNotify = Notifies[0];
	if (AnimStartNotify)
	{
		AnimStartNotify->OnActionPointReached.BindUObject(this, &UBaseCharacterMovementComponent::AimStartFinished);
	}
	if (AimMont && AimMontage)
	{
		const float PlayRate = AnimInst->Montage_GetPlayRate(AimMontage);
		Seconds = AnimInst->Montage_GetPosition(AimMontage);
		if (Seconds<AimMontage->GetPlayLength() && PlayRate==1)
		{
			AnimInst->Montage_Stop(0.1, AimMontage);
			
		}
	}
	AnimInst->Montage_Play(AimMontage, -1, EMontagePlayReturnType::MontageLength,
				Seconds, true);*/
	PlayerMovementLogic.IsPivotTargeted=false;
	UE_LOG(LogPlayerMovement, Warning, TEXT("Yaw is %f, Pitch is %f"), CharacterOwner->GetController()->K2_GetActorRotation().Yaw, CharacterOwner->GetController()->K2_GetActorRotation().Pitch);
	UE_LOG(LogPlayerMovement, Warning, TEXT("Yaw is %f, Pitch is %f"), CharacterOwner->K2_GetActorRotation().Yaw, CharacterOwner->K2_GetActorRotation().Pitch);
}

FPlayerMovementLogic UBaseCharacterMovementComponent::GetPlayerMovementLogic()
{
	return PlayerMovementLogic;
}

