// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/GrenadeTask_ThrowGrenade.h"
#include "GrenadeAbility.generated.h"

UCLASS()
class PROJECTREVIVAL_API UGrenadeAbility : public UPRGameplayAbility
{
	GENERATED_BODY()
public:
	UGrenadeAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="General Settings")
	float ThrowForce = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="General Settings")
	FName GrenadeSocketName = "GrenadeSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General Settings")
	TSubclassOf<ABaseGrenade> CurrentGrenade = ABaseGrenade::StaticClass();

	// Animation of throwing grenade from idle position
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General Settings")
	// UAnimMontage* IdleThrowAnimation;

protected:
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY()
	UGrenadeTask_ThrowGrenade* GrenadeTask;
};
