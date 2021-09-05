// Project Revival. All Rights Reserved


#include "AI/Services/ChangeWeaponService.h"
#include "AIController.h"
#include "PRUtils.h"
#include "WeaponComponent.h"

UChangeWeaponService::UChangeWeaponService()
{
	NodeName = "ChangeWeapon";
}

void UChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (Controller)
	{
		const auto WeaponComponent = PRUtils::GetCharacterComponent<UWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent && FMath::Rand()<=Probability && Probability>0)
		{
			WeaponComponent->NextWeapon();
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
