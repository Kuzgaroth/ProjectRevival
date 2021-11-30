#pragma once
#include "PRAbilityTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPRAbilitySystemBase, Log, All)

UENUM(BlueprintType)
enum class EGASInputActions : uint8
{
	None,
	Flip,
	Visor,
	Ghost
};