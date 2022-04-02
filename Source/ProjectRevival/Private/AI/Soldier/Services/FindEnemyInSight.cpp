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
	FPlayerPositionData PlayerPos;
	if (PerceptionComponent)
	{
		PlayerPos = PerceptionComponent->GetClosestEnemy();
		if (PlayerPos.GetActor())
		{
			Controller->SetBIsPlayerInSight(true);
		}
		else
		{
			Controller->SetBIsPlayerInSight(false);
		}
		Controller->SetPlayerPos(PlayerPos);
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
