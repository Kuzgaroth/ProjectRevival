// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/VampireAbility_TraceTask.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Particles/ParticleSystemComponent.h"
#include "VampireAbility.generated.h"

UCLASS()
class PROJECTREVIVAL_API UVampireAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UVampireAbility();
	    
	UPROPERTY(EditDefaultsOnly, Category="Ability constants")
	float VampireAbilityDistance = 5000.f;
    
	UPROPERTY(EditDefaultsOnly, Category="Ability constants")
	float VampireAbilityDamage = 50.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* VampireMontage;

	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* VampireReverseMontage;

	UPROPERTY(EditDefaultsOnly, Category="Animations")
	float MontageRate = 3.0;
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* BeamFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystemComponent* BeamComp;
    	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TArray<UParticleSystemComponent*>BeamArray;
	
protected:	
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnTraceAnalysisEnd();
	UFUNCTION()
	void OnReverseMontageEnd();
	UFUNCTION()
	void PlayVFX();
	UFUNCTION()
	void MakeDamage(ABaseCharacter* DamagedActor, APlayerCharacter* Character, APlayerController* Controller) const;
	
	float MontageDuration;
private:
	UPROPERTY()
	UVampireAbility_TraceTask* TraceTask;
	
	UPROPERTY()
	UAbilityTask_WaitDelay* DelayTask;
	
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageTask;
};