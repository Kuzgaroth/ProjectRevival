// Project Revival. All Rights Reserved


#include "AI/Soldier/SoldierAIController.h"
#include "AI/AICharacter.h"
#include "BasePickup.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RespawnComponent.h"

ASoldierAIController::ASoldierAIController()
{
	PRPerceptionComponent = CreateDefaultSubobject<UPRSoldierAIPerceptionComponent>("PRPerceptionComponent");
	SetPerceptionComponent(*PRPerceptionComponent);

	RespawnComponent = CreateDefaultSubobject<URespawnComponent>("RespawnController");
	
	bWantsPlayerState = true;
	bIsFiring = false;
}

void ASoldierAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto AICharacter = Cast<AAICharacter>(InPawn);
	UE_LOG(LogTemp, Log, TEXT("Character Possessed"))
	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->BehaviorTreeAsset);
	}
}

void ASoldierAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

void ASoldierAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoldierAIController::StartFiring()
{
	StartFiringAtPlayerPos.Broadcast(PlayerPos);
}

void ASoldierAIController::StopFiring()
{
	SetBIsFiring(false);
}

AActor* ASoldierAIController::GetFocusOnActor()
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
