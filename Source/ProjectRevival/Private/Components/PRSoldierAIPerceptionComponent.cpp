// Project Revival. All Rights Reserved


#include "Components/PRSoldierAIPerceptionComponent.h"
#include "PRUtils.h"
#include "HealthComponent.h"
#include "AIController.h"
#include "Player/PlayerCharacter.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

DEFINE_LOG_CATEGORY(LogPRAIPerception);

AActor* UPRSoldierAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
	if (PerceiveActors.Num()==0)
	{
		UE_LOG(LogPRAIPerception, Log, TEXT("Empty Sight"))
		GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), PerceiveActors);
	}
	if (PerceiveActors.Num()==0)
	{
		UE_LOG(LogPRAIPerception, Log, TEXT("Empty Hearing"))
		return nullptr;
	}
	UE_LOG(LogPRAIPerception, Log, TEXT("Not empty"))

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;

	for (const auto Actor : PerceiveActors)
	{
		const auto HealthComponent = PRUtils::GetCharacterComponent<UHealthComponent>(Actor);
		const auto PerceivePawn = Cast<APawn>(Actor);
		const auto AreEnemies = PerceivePawn && PRUtils::AreEnemies(Controller, PerceivePawn->Controller);
		if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
		{
			const auto CurrentDistance = (Actor->GetActorLocation()-Pawn->GetActorLocation()).Size();
			if (CurrentDistance<BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = Actor;
			}
		}
	}
	return BestPawn;
}
