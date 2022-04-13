// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/DimensionShotTask_GiveRevolver.h"
#include "DimensionRevolver.h"
#include "DimensionRevolverBullet.h"
#include "PlayerCharacter.h"

void UDimensionShotTask_GiveRevolver::Activate()
{
	Super::Activate();
	bTickingTask=true;
}

UDimensionShotTask_GiveRevolver* UDimensionShotTask_GiveRevolver::DimensionShotTaskInit(UGameplayAbility* OwningAbility,
                                                                                        TSubclassOf<ABaseWeapon> newRevolverData, float healtpercent)
{
	const auto AbilityTask=NewAbilityTask<UDimensionShotTask_GiveRevolver>(OwningAbility,FName("GiveRevolverTask"));
	AbilityTask->RevolverData=newRevolverData;
	AbilityTask->HealPercent=healtpercent;
	return AbilityTask;
}

ABaseWeapon* UDimensionShotTask_GiveRevolver::StartTask()
{
	auto player=Cast<APlayerCharacter>(GetOwnerActor());
	player->CameraZoomIn();
	player->PlayerAimZoom.IsZooming=false;
	const auto WeaponComponent=player->GetWeaponComponent();
	
	WeaponComponent->AddWeapon(RevolverData);
	WeaponComponent->EquipWeapon(WeaponComponent->GetWeaponNum());
	auto weapon=WeaponComponent->GetCurrentWeapon();
	Super::Activate();
	return weapon;
}


