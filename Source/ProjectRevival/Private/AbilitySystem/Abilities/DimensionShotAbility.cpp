// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/DimensionShotAbility.h"

#include "BaseCharacter.h"
#include "DimensionRevolver.h"
#include "DimensionRevolverBullet.h"
#include "WeaponComponent.h"

bool UDimensionShotAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                               const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UDimensionShotAbility::ShotWasMade()
{
	ShotBeingMade=true; 
}

void UDimensionShotAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
}

void UDimensionShotAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDimensionShotAbility::FinishAbility()
{
	
}
