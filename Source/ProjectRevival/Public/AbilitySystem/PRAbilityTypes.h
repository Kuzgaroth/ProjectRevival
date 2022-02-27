#pragma once
#include "PRAbilityTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPRAbilitySystemBase, Log, All)

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEnergyValueChanged, float, NewEnergyValue);

UENUM(BlueprintType)
enum class EGASInputActions : uint8
{
	None,
	Flip,
	Visor,
	Ghost,
	ChangeWorld,
	MeleeAttack
};