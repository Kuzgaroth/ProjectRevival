// Project Revival. All Rights Reserved


#include "Pickup/HealthPickup.h"
#include "Components/HealthComponent.h"
#include "ProjectRevival/Public/Miscellaneous/PRUtils.h"

void AHealthPickup::OnInteraction_Implementation(AActor* Actor)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, "Ouch!");
}

bool AHealthPickup::GivePickUpTo(APawn* PlayerPawn)
{
	const auto HealthComponent = PRUtils::GetCharacterComponent<UHealthComponent>(PlayerPawn);
	if (!HealthComponent) return false;
	return HealthComponent->TryToAddHealth(HealthAmount);
}
