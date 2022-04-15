// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "VisorPlacerTask_SpawnVisor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UVisorPlacerTask_SpawnVisor : public UAbilityTask
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UVisorPlacerTask_SpawnVisor* SpawnVisor(UGameplayAbility* OwningAbility, float VisorRadius, float VisorDuration,
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToVisor, UParticleSystem* ParticleEffect);
	
	//The range in which enemies and objects are visored 
	UPROPERTY()
	float Radius;
	
	//Lifetime of ability
	UPROPERTY()
	float Duration;

	UPROPERTY()
	UGameplayAbility* AbilityHead;

	//Types of collisions by which objects are going to be visored
	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForVisoring;

	// Particle effect spawned visor point (item)
	UPROPERTY()
	UParticleSystem* EffectToShow;
	
private:
	virtual void OnDestroy(bool AbilityEnded) override;
};
