// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "PRSoldierAIPerceptionComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPRAIPerception, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRSoldierAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetClosestEnemy() const;
	bool GetBestCoverWing(EWing Wing, FVector& CoverPos);
};
