// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "DimensionShotTask_GiveRevolver.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UDimensionShotTask_GiveRevolver : public UAbilityTask
{
	GENERATED_BODY()
private:
	virtual void Activate() override;
	FWeaponData RevolverData;
public:
	UDimensionShotTask_GiveRevolver* DimensionShotTaskInit(UGameplayAbility* OwningAbility, FWeaponData newRevolverData);
};
