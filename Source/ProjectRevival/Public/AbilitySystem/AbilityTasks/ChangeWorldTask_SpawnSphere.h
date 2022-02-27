// Project Revival. All Rights Reserved

#pragma once



#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameFramework/Actor.h"
#include "ChangeWorldTask_SpawnSphere.generated.h"

class AChangeWorldSphereActor;
/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UChangeWorldTask_SpawnSphere : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
    static UChangeWorldTask_SpawnSphere* ChangeWorldInit(UGameplayAbility* OwningAbility,TSubclassOf<AActor> ChangeWorldSphere,float TraceDistance);
    	
    AChangeWorldSphereActor* StartTask(AActor& OwnerLocation);
private:
	UPROPERTY()
	TSubclassOf<AActor> ChangeWorldSphere;
	UPROPERTY()
	float SpawnSphereDistance;


};
