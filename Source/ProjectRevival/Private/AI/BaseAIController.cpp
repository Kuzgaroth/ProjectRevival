// Project Revival. All Rights Reserved


#include "AI/BaseAIController.h"
#include "AICharacter.h"
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
	SetFocus(AimActor);
}

AActor* ABaseAIController::GetFocusOnActor()
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
