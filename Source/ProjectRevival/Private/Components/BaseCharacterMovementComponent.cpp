// Project Revival. All Rights Reserved


#include "Components/BaseCharacterMovementComponent.h"
#include "Player/BaseCharacter.h"

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ABaseCharacter* Player = Cast<ABaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed*RunModifier:MaxSpeed;
}
