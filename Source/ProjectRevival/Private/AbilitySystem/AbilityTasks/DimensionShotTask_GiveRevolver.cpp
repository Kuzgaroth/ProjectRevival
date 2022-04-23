// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/DimensionShotTask_GiveRevolver.h"
#include "DimensionRevolver.h"
#include "PlayerCharacter.h"
#include "AbilitySystem/Abilities/DimensionShotAbility.h"

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
	player->DimensionShotAbStruct.IsInRevolverAim=true;
	player->DimensionShotAbStruct.Ability=Cast<UDimensionShotAbility>(Ability);
	const auto WeaponComponent=player->GetWeaponComponent();
	
	WeaponComponent->AddWeapon(RevolverData);
	WeaponComponent->GetCurrentWeapon()->IsAppearing=false;
	WeaponComponent->GetCurrentWeapon()->Changing();
	WeaponComponent->EquipWeapon(WeaponComponent->GetWeaponNum());
	WeaponComponent->GetCurrentWeapon()->IsAppearing=true;
	WeaponComponent->GetCurrentWeapon()->Changing();
	auto weapon=WeaponComponent->GetCurrentWeapon();
	Super::Activate();
	return weapon;
}


