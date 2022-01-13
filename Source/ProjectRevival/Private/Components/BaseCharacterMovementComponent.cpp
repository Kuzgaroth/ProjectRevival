// Project Revival. All Rights Reserved


#include "Components/BaseCharacterMovementComponent.h"
#include "AnimUtils.h"
#include "JumpAnimNotify.h"
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
		JumpEndedNotify->OnJumpStarted.BindUObject(this, &UBaseCharacterMovementComponent::JumpProcessEnded);
	}
	CharacterOwner->Jump();
	
}

void UBaseCharacterMovementComponent::JumpProcessEnded()
{
	PlayerMovementLogic.JumpStarted=false;
	PlayerMovementLogic.JumpPressed=false;
}

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ABaseCharacter* Player = Cast<ABaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed*RunModifier:MaxSpeed;
}

void UBaseCharacterMovementComponent::TurnLeft(bool IsPivotTargeted)
{
}

void UBaseCharacterMovementComponent::TurnRight(bool IsPivotTargeted)
{
}

void UBaseCharacterMovementComponent::TurnBackward(bool IsPivotTargeted)
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
		JumpStartedNotify->OnJumpStarted.BindUObject(this, &UBaseCharacterMovementComponent::JumpPressEnded);
	}
}

FPlayerMovementLogic UBaseCharacterMovementComponent::GetPlayerMovementLogic()
{
	return PlayerMovementLogic;
}

