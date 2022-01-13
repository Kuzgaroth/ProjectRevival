// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "CirclePBWidget.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/PRAbilityTypes.h"
#include "PRGameplayAbility.generated.h"


UCLASS()
class PROJECTREVIVAL_API UPRGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPRGameplayAbility();
protected:
	//Необходимо ввести С++ тип виджета способности
	//Тип виджета способности AbilityWidget;
	UPROPERTY()
	UCirclePBWidget* AbilityWidget;

	UPROPERTY(EditDefaultsOnly)
	EGASInputActions AbilityAction;
	
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
