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
	
	UPROPERTY(EditDefaultsOnly, Category="Melee Attack Visual")
	UCurveFloat* AttackCurve;
    
	UPROPERTY(EditDefaultsOnly, Category="Melee Attack Duration")
	float AttackDuration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Melee Attack Damage")
	float AttackDamage = 10.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Melee Attack Animation")
	UAnimMontage* Montage;
	
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Melee Attack Animation")
	FName MeleeAttackMontage;
protected:	
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnAttackBegin();
	UFUNCTION()
	void OnAttackEnd();
	
private:
	UPROPERTY()
	UMeleeAttackTask_Hit* AttackTask;
	UPROPERTY()
	UAbilityTask_WaitDelay* DelayTask;
};