// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PRAbilitySystemComponent.generated.h"



UCLASS()
class PROJECTREVIVAL_API UPRAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPRAbilitySystemComponent();
	
	//void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UPRGameplayAbility*>& ActiveAbilities);
	
	int32 GetDefaultAbilityLevel() const;
	
	static UPRAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);
};
