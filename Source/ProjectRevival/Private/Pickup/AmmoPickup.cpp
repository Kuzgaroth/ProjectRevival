// Project Revival. All Rights Reserved


#include "Pickup/AmmoPickup.h"
#include "Components/WeaponComponent.h"
#include "Components/HealthComponent.h"
#include "ProjectRevival/Public/Miscellaneous/PRUtils.h"

bool AAmmoPickup::GivePickUpTo(APawn* PlayerPawn)
{
	const auto HealthComponent = PRUtils::GetCharacterComponent<UHealthComponent>(PlayerPawn);
	if (!HealthComponent || HealthComponent->IsDead()) return false;
	const auto WeaponCompoent = PRUtils::GetCharacterComponent<UWeaponComponent>(PlayerPawn);
	if (!WeaponCompoent) return false;
	
	return WeaponCompoent->TryToAddAmmo(WeaponType, ClipsAmount);
}
