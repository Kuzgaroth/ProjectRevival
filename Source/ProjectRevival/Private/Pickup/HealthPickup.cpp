// Project Revival. All Rights Reserved


#include "Pickup/HealthPickup.h"
#include "Components/HealthComponent.h"
#include "ProjectRevival/Public/Miscellaneous/PRUtils.h"

bool AHealthPickup::GivePickUpTo(APawn* PlayerPawn)
{
	const auto HealthComponent = PRUtils::GetCharacterComponent<UHealthComponent>(PlayerPawn);
	if (!HealthComponent) return false;
	return HealthComponent->TryToAddHealth(HealthAmount);
}
