// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/AbilityTasks/MeleeAttackTask_Hit.h"
#include "MeleeAttackAbility.generated.h"

UCLASS()
class PROJECTREVIVAL_API UMeleeAttackAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UMeleeAttackAbility();
	
	UPROPERTY(EditDefaultsOnly, Category="Constants")
	UCurveFloat* AttackCurve;
    
	UPROPERTY(EditDefaultsOnly, Category="Constants")
	float AttackDuration = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Constants")
	float AttackDamage = 50.0;
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animations")
	UAnimMontage* AttackMontage1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animations")
	UAnimMontage* AttackMontage2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animations")
	UAnimMontage* AttackMontage3;

	UPROPERTY(BlueprintReadOnly, Category="Animations")
	UAnimMontage* AttackMontage;
protected:	
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnDelayEnd();
	/*
	UFUNCTION()
	void OnAttackBegin();
	
	*/
private:
	UPROPERTY()
	UMeleeAttackTask_Hit* AttackTask;
	UPROPERTY()
	UAbilityTask_WaitDelay* DelayTask;
};