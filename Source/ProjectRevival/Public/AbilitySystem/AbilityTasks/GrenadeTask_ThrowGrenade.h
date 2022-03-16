// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/AbilityActors/Grenades/BaseGrenade.h"
#include "GrenadeTask_ThrowGrenade.generated.h"

UCLASS()
class PROJECTREVIVAL_API UGrenadeTask_ThrowGrenade : public UAbilityTask
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UGrenadeTask_ThrowGrenade* ThrowGrenade(UGameplayAbility* OwningAbility, TSubclassOf<ABaseGrenade> GrenadeToUse, float ThrowGrenadeForce, FName GrenadeSocketName);

	UPROPERTY()
	UGameplayAbility* AbilityHead;

protected:
	UPROPERTY()
	TSubclassOf<ABaseGrenade> GrenadeToUsePassed;

	UPROPERTY()
	float ThrowGrenadeForcePassed;

	UPROPERTY()
	FName GrenadeSocketNamePassed;
	
private:
	virtual void OnDestroy(bool AbilityEnded) override;
};
