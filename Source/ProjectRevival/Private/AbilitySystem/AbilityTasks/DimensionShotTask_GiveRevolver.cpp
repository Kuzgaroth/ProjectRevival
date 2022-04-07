// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/DimensionShotTask_GiveRevolver.h"

void UDimensionShotTask_GiveRevolver::Activate()
{
	Super::Activate();
	
}

UDimensionShotTask_GiveRevolver* UDimensionShotTask_GiveRevolver::DimensionShotTaskInit(UGameplayAbility* OwningAbility,
                                                                                        FWeaponData newRevolverData)
{
	const auto AbilityTask=NewAbilityTask<UDimensionShotTask_GiveRevolver>(OwningAbility,FName("GiveRevolverTask"));
	AbilityTask->RevolverData=newRevolverData;
	return AbilityTask;
}
