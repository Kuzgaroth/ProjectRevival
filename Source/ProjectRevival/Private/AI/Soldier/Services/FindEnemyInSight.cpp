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
		UE_LOG(LogPRAITasks, Log, TEXT("From Task"))
		if (PlayerPos.GetActor() != nullptr)
		{
			Controller->SetPlayerPos(PlayerPos, false);
			Controller->SetBIsPlayerInSight(true);
		}
		else
		{
			Controller->SetBIsPlayerInSight(false);
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
