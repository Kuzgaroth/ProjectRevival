// Project Revival. All Rights Reserved


#include "AI/BaseAIController.h"
#include "AICharacter.h"
#include "BasePickup.h"
#include "HealthPickup.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/BotPickupInterface.h"
#include "Components/PRAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RespawnComponent.h"

ABaseAIController::ABaseAIController()
{
	PRPerceptionComponent = CreateDefaultSubobject<UPRAIPerceptionComponent>("PRPerceptionComponent");
	SetPerceptionComponent(*PRPerceptionComponent);

	RespawnComponent = CreateDefaultSubobject<URespawnComponent>("RespawnController");
	
	bWantsPlayerState = true;
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto AICharacter = Cast<AAICharacter>(InPawn);
	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->BehaviorTreeAsset);
	}
	
}

void ABaseAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const auto AimActor = GetFocusOnActor();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHealthPickup::StaticClass(), FoundActors);
	for (const auto Actor : FoundActors)
	{
		IBotPickupInterface* Interface = Cast<IBotPickupInterface>(Actor);
		if(Interface)
		{
			Blackboard->SetValueAsObject("HealthPickup", Actor);
			//Interface->Execute_OnInteraction(Actor, this);
		}
	}
	SetFocus(AimActor);
}

AActor* ABaseAIController::GetFocusOnActor()
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
