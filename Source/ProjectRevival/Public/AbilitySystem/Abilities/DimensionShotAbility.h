// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/DimensionShotTask_GiveRevolver.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "DimensionShotAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UDimensionShotAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UDimensionShotAbility();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	UFUNCTION()
	void ShotWasMade();
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	void InterruptAbility();
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	UPROPERTY(EditDefaultsOnly, Category="AbilityWeapon")
	TSubclassOf<ABaseWeapon> DimensionRevolverData;
	UPROPERTY(EditDefaultsOnly, Category="HealOfPlayer")
	float HealPercent=30.0f;

private:
	UDimensionShotTask_GiveRevolver *GiveRevolverTask;
	void FinishAbility();
	bool ShotBeingMade=false;
	
	
};
