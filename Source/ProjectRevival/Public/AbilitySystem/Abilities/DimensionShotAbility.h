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
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	void ShotWasMade();
	
protected:
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	UPROPERTY(EditDefaultsOnly, Category="AbilityWeapon")
	FWeaponData DimensionRevolverData;
private:
	UDimensionShotTask_GiveRevolver *GiveRevolverTask;
	void FinishAbility();
	bool ShotBeingMade=false;
	
	
};
