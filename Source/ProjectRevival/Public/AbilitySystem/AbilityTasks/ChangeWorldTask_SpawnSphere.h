// Project Revival. All Rights Reserved

#pragma once



#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameFramework/Actor.h"
#include "ChangeWorldTask_SpawnSphere.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UChangeWorldTask_SpawnSphere : public UAbilityTask
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TSubclassOf<AActor> ChangeWorldSphere;
public:
	virtual void Activate() override;
};
