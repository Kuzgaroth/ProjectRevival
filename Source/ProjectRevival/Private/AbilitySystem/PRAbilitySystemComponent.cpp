// Project Revival. All Rights Reserved


#include "AbilitySystem/PRAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "BaseCharacter.h"

UPRAbilitySystemComponent::UPRAbilitySystemComponent() {}

int32 UPRAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	//ABaseCharacter* OwningCharacter = Cast<ABaseCharacter>(GetOwnerActor());

	/*if (OwningCharacter)
	{
		return OwningCharacter->GetCharacterLevel();
	}*/
	
	return 1;
}

UPRAbilitySystemComponent* UPRAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor,
	bool LookForComponent)
{
	return Cast<UPRAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
