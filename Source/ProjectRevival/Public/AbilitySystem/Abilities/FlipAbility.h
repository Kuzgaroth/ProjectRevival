// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/FlipTask_FlipToggle.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "FlipAbility.generated.h"

UCLASS()
class PROJECTREVIVAL_API UFlipAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UFlipAbility();
	
	UPROPERTY(EditDefaultsOnly, Category="Flip Visual")
	UCurveFloat* FlipCurve;
    
	UPROPERTY(EditDefaultsOnly, Category="Flip constants")
	float FlipDuration;
    
	UPROPERTY(EditDefaultsOnly, Category="Flip constants")
	float FlipStrength;
	
	UPROPERTY(BlueprintReadOnly, Category="Flip constants")
	FVector FlipDirection;
	
	UPROPERTY(EditDefaultsOnly, Category="Flip Visual")
	UAnimMontage* ForwardMontage;
	UPROPERTY(EditDefaultsOnly, Category="Flip Visual")
	UAnimMontage* BackwardMontage;
	UPROPERTY(EditDefaultsOnly, Category="Flip Visual")
	UAnimMontage* RightMontage;
	UPROPERTY(EditDefaultsOnly, Category="Flip Visual")
	UAnimMontage* LeftMontage;
	UPROPERTY(EditDefaultsOnly, Category="Flip Visual")
	UAnimMontage* ForwardRightMontage;
	UPROPERTY(EditDefaultsOnly, Category="Flip Visual")
	UAnimMontage* BackwardRightMontage;
	UPROPERTY(EditDefaultsOnly, Category="Flip Visual")
	UAnimMontage* ForwardLeftMontage;
	UPROPERTY(EditDefaultsOnly, Category="Flip Visual")
	UAnimMontage* BackwardLeftMontage;
	UPROPERTY(BlueprintReadOnly, Category="Flip Visual")
	UAnimMontage* FlipMontage;	
protected:	
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	UFUNCTION()
	void OnFlipEnd();
	UFUNCTION()
	bool GetDirectionalMontage(const APlayerCharacter* Character, const  APlayerController* Controller);
	
private:
	UPROPERTY()
	UFlipTask_FlipToggle* FlipTask;
	UPROPERTY()
	UAbilityTask_WaitDelay* DelayTask;
};
