// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "VisorSwitcherTask_TriggerVisor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UVisorSwitcherTask_TriggerVisor : public UAbilityTask
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UVisorSwitcherTask_TriggerVisor* SpawnVisor(UGameplayAbility* OwningAbility, float VisorRadius, float VisorDuration,
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToVisor, UParticleSystem* ParticleEffect, class UMaterial* ReplaceMaterial);
	
	//The range in which enemies and objects are visored 
	UPROPERTY()
	float Radius;
	
	//Lifetime of ability
	UPROPERTY()
	float Duration;

	UPROPERTY()
	UGameplayAbility* AbilityHead;

	UPROPERTY()
	class UMaterial* MaterialToSet;

	//Types of collisions by which objects are going to be visored
	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForVisoring;

	// Particle effect spawned visor point (item)
	UPROPERTY()
	UParticleSystem* EffectToShow;

	UPROPERTY()
	AActor* SpawnedVisorRef;
	
private:
	virtual void OnDestroy(bool AbilityEnded) override;
};
