// Project Revival. All Rights Reserved


#include "Components/BaseCharacterMovementComponent.h"
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
	if (PlayerMovementLogic.JumpStarted || PlayerMovementLogic.JumpPressed) return;
	PlayerMovementLogic.JumpPressed = true;
	auto JumpStartedNotify = AnimUtils::FindNotifyByClass<UJumpAnimNotify>(JumpAnim);
	if (JumpStartedNotify)
	{
		JumpStartedNotify->OnActionPointReached.BindUObject(this, &UBaseCharacterMovementComponent::JumpPressEnded);
	}
}

FPlayerMovementLogic UBaseCharacterMovementComponent::GetPlayerMovementLogic()
{
	return PlayerMovementLogic;
}

