// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/VisorSwitcherTask_TriggerVisor.h"
#include "VisorSwitcherAbility.generated.h"


UCLASS()
class PROJECTREVIVAL_API UVisorSwitcherAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UVisorSwitcherAbility();
	
	//The range in which enemies and objects are visored 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Visor")
	float VisorRadius = 1000.0f;
	
	//Lifetime of ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Visor")
	float VisorDuration = 5.0f;

	//Types of collisions by which objects are going to be visored
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Visor")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToVisor;

	// Animation of ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Visor")
	UAnimMontage* UseVisorAnim;

	// Particle effect spawned visor point (item)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Visor")
	UParticleSystem* ParticleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Visor")
	class UMaterial* OnMaterial;
	
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY()
	UVisorSwitcherTask_TriggerVisor* VisorTaskSwitcher;
};
