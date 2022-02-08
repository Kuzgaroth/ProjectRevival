// Project Revival. All Rights Reserved


#include "AI/Soldier/SoldierAIController.h"
#include "AI/AICharacter.h"
#include "BasePickup.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RespawnComponent.h"
#include "SoldierEnemy.h"
#include "GameFeature/CoverObject.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

DEFINE_LOG_CATEGORY(LogPRAIController)

ASoldierAIController::ASoldierAIController()
{
	PRPerceptionComponent = CreateDefaultSubobject<UPRSoldierAIPerceptionComponent>("PRPerceptionComponent");
	const auto SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(15.0f);
	PRPerceptionComponent->ConfigureSense(*SightConfig);

	const auto HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing Config");
	HearingConfig->HearingRange = 3000.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->SetMaxAge(15.0f);
	PRPerceptionComponent->ConfigureSense(*HearingConfig);
	
	PRPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

	SetPerceptionComponent(*PRPerceptionComponent);

	RespawnComponent = CreateDefaultSubobject<URespawnComponent>("RespawnController");

	/*
	const auto Character = Cast<ASoldierEnemy>(GetCharacter());
	PlayerPosDelegate.AddDynamic(ASoldierEnemy::StaticClass(), ASoldierEnemy::StartFiring(PlayerPos));
	*/

	SideMovementAmount = 0;
	
	bWantsPlayerState = true;
	bIsFiring = false;
	bIsInCover = false;
	bIsSideTurning = false;

	BotWing = EWing::Center;
}

void ASoldierAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto AISoldier = Cast<ASoldierEnemy>(InPawn);
	UE_LOG(LogPRAIController, Log, TEXT("Character Possessed"))
	if (AISoldier)
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
	Cast<ASoldierEnemy>(GetPawn())->StopEnteringCoverDelegate.AddDynamic(this, &ASoldierAIController::StopEnteringCover);
	Cast<ASoldierEnemy>(GetPawn())->StopExitingCoverDelegate.AddDynamic(this, &ASoldierAIController::StopExitingCover);
	Cast<ASoldierEnemy>(GetPawn())->StopCoverSideMovingDelegate.AddDynamic(this, &ASoldierAIController::StopCoverSideMoving);
}

void ASoldierAIController::StartFiring()
{
	UE_LOG(LogPRAIController, Log, TEXT("Player pos X: %0.2f, Y: %0.2f, Z: %0.2f"), PlayerPos.X, PlayerPos.Y, PlayerPos.Z);
	PlayerPosDelegate.Broadcast(PlayerPos);
}

void ASoldierAIController::StopFiring()
{
	SetBIsFiring(false);
}

void ASoldierAIController::StartEnteringCover()
{
	const auto PawnPos = GetPawn()->GetActorLocation();
	CoverPos = PRPerceptionComponent->GetBestCoverWing(BotWing);
	/*const auto BlackboardComp = GetBlackboardComponent();*/
	UE_LOG(LogPRAIController, Log, TEXT("Pawn pos X: %0.2f, Y:%0.2f"), PawnPos.X, PawnPos.Y);
	UE_LOG(LogPRAIController, Log, TEXT("%i Cover pos X: %0.2f, Y: %0.2f"), BotWing, CoverPos.X, CoverPos.Y);
	StartEnteringCoverDelegate.Broadcast(CoverPos);
	/*if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(CoverKeyname, CoverPos);
	}*/
}

void ASoldierAIController::StopEnteringCover()
{
	bIsInCover = true;
}

void ASoldierAIController::StartExitingCover()
{
	StartExitingCoverDelegate.Broadcast();
}

void ASoldierAIController::StopExitingCover()
{
	bIsInCover = false;
}

void ASoldierAIController::StartCoverSideMoving()
{
	bIsSideTurning = true;
	//SideMovementAmount defines the desired movement distance while in cover
	StartCoverSideMovingDelegate.Broadcast(SideMovementAmount);
}

void ASoldierAIController::StopCoverSideMoving()
{
	bIsSideTurning = false;
}

AActor* ASoldierAIController::GetFocusOnActor()
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
