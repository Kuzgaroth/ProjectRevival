// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "PRSoldierAIPerceptionComponent.generated.h"

// Структура для распределения ботов по направлениям


/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRSoldierAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	FPlayerPositionData GetClosestEnemy() const;
	bool GetBestCoverWing(EWing Wing, FVector& CoverPos, AActor*& CoverRef);
};
