// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/GhostTask_InvisibilityToggle.h"
#include "GhostAbility.generated.h"



UCLASS()
class PROJECTREVIVAL_API UGhostAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UGhostAbility();
protected:
	UPROPERTY(EditDefaultsOnly, Category="Duration")
	float Duration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Ghost Visual")
	UCurveFloat* VisualCurve;
	
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	UFUNCTION()
	void OnAppearEnded();
	UFUNCTION()
	void OnDisappearEnded();
private:
	UPROPERTY()
	UGhostTask_InvisibilityToggle* GhostTask;
	UPROPERTY()
	UAbilityTask_WaitDelay* DelayTask;

	UFUNCTION()
	void BeginAppear();
};
