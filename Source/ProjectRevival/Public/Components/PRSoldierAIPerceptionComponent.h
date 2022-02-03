// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Perception/AIPerceptionComponent.h"
#include "PRSoldierAIPerceptionComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPRAIPerception, Log, All);

// Структура для распределения ботов по направлениям
UENUM(BlueprintType)
enum class EWing: uint8
{
	Left = 0,
	Center = 1,
	Right = 2
};

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRSoldierAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetClosestEnemy() const;
	FVector GetBestCoverWing(EWing Wing);
};
