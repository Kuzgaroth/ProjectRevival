// Project Revival. All Rights Reserved


#include "AI/Soldier/Services/FindEnemyInSight.h"

#include "AIController.h"
#include "PRSoldierAIPerceptionComponent.h"
#include "PRUtils.h"
#include "Soldier/SoldierAIController.h"

UFindEnemyInSight::UFindEnemyInSight()
{
	NodeName = "Find enemy in sight";
}

void UFindEnemyInSight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	const auto PerceptionComponent = Cast<UPRSoldierAIPerceptionComponent>(Controller->GetPerceptionComponent());
	if (PerceptionComponent)
	{
		Controller->SetPlayerPos(PerceptionComponent->GetClosestEnemy());
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
