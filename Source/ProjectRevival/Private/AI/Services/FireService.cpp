// Project Revival. All Rights Reserved


#include "AI/Services/FireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PRUtils.h"
#include "Components/WeaponComponent.h"

UFireService::UFireService()
{
	NodeName = "Fire";
}

void UFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	const auto bHasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName)!=nullptr;
	if (Controller)
	{
		const auto WeaponComponent = PRUtils::GetCharacterComponent<UWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent)
		{
			bHasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

void UFireService::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	const auto bHasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);
	if (Controller)
	{
		const auto WeaponComponent = PRUtils::GetCharacterComponent<UWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent)
		{
			if (!bHasAim) WeaponComponent->StopFire();
		}
	}
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}
