// Project Revival. All Rights Reserved


#include "AI/Soldier/SoldierAIController.h"
#include "AI/AICharacter.h"
#include "BasePickup.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RespawnComponent.h"
#include "SoldierEnemy.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

DEFINE_LOG_CATEGORY(LogPRAIController);
DEFINE_LOG_CATEGORY(LogPRAIDecorators);
DEFINE_LOG_CATEGORY(LogPRAITasks);

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
	
	SideMovementAmount = 0;
	bWantsPlayerState = true;
	bIsFiring = false;
	bIsInCover = false;
	bIsSideTurning = false;
	BotWing = EWing::Center;
}

void ASoldierAIController::SetPlayerPos(const FPlayerPositionData &NewPlayerPos)
{
	if (NewPlayerPos.GetActor())
	{
		PlayerPos.SetActor(NewPlayerPos.GetActor());
	}
	if (NewPlayerPos.GetCover())
	{
		PlayerPos.SetCover(NewPlayerPos.GetCover());
	}
	OnPlayerSpotted.Broadcast(PlayerPos);
}

void ASoldierAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto AIChar = Cast<AAICharacter>(InPawn);
	//UE_LOG(LogPRAIController, Log, TEXT("Character Possessed"));
	if (AIChar)
	{
		//UE_LOG(LogPRAIController, Log, TEXT("BehaviorTree started"));
		RunBehaviorTree(AIChar->BehaviorTreeAsset);
		Cast<ASoldierEnemy>(GetPawn())->StopEnteringCoverDelegate.AddDynamic(this, &ASoldierAIController::StopEnteringCover);
		Cast<ASoldierEnemy>(GetPawn())->StopExitingCoverDelegate.AddDynamic(this, &ASoldierAIController::StopExitingCover);
		Cast<ASoldierEnemy>(GetPawn())->StopCoverSideMovingDelegate.AddDynamic(this, &ASoldierAIController::StopCoverSideMoving);
		Cast<ASoldierEnemy>(GetPawn())->StartFireDelegate.AddDynamic(this, &ASoldierAIController::StartFiring);
		Cast<ASoldierEnemy>(GetPawn())->StopFireDelegate.AddDynamic(this, &ASoldierAIController::StopFiring);
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

void ASoldierAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnBotDied.Clear();
	OnPlayerSpotted.Clear();
	if (OnBotWingDecision.IsBound()) OnBotWingDecision.Unbind();
}

void ASoldierAIController::StartFiring()
{
	const auto PlayerCoordinates = PlayerPos.GetActorPosition();
	UE_LOG(LogPRAIController, Log, TEXT("Shoot at Player pos X: %0.2f, Y: %0.2f, Z: %0.2f"), PlayerCoordinates.X, PlayerCoordinates.Y);
	PlayerPosDelegate.Broadcast(PlayerPos);
}

void ASoldierAIController::StopFiring()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Controller: StopFiring() was called"));
	SetBIsFiring(false);
}

void ASoldierAIController::StartEnteringCover()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Controller: StartEnteringCover() was called"));
	UE_LOG(LogPRAIController, Log, TEXT("Controller: %i Cover pos X: %0.2f, Y: %0.2f"), BotWing, CoverPos.X, CoverPos.Y);
	StartEnteringCoverDelegate.Broadcast(CoverPos, CoverRef);
}

void ASoldierAIController::StopEnteringCover()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Controller: StopEnteringCover() was called"));
	SetBIsInCover(true);
}

void ASoldierAIController::StartExitingCover()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Controller: StartExitingCover() was called"));
	StartExitingCoverDelegate.Broadcast();
}

void ASoldierAIController::StopExitingCover()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Controller: StopExitingCover() was called"));
	SetBIsInCover(false);
}

void ASoldierAIController::StartCoverSideMoving()
{
	SetBIsSideTurning(true);
	//SideMovementAmount defines the desired movement distance while in cover
	StartCoverSideMovingDelegate.Broadcast(SideMovementAmount);
}

void ASoldierAIController::StopCoverSideMoving()
{
	SetBIsSideTurning(false);
}

bool ASoldierAIController::FindNewCover()
{
	UE_LOG(LogPRAISoldier, Warning, TEXT("Controller: Bot doing this is %s"), *GetPawn()->GetName())
	bool const bFlag = PRPerceptionComponent->GetBestCoverWing(BotWing, CoverPos, CoverRef);
	const auto BlackboardComp = GetBlackboardComponent();
	if (bFlag && BlackboardComp)
	{
		UE_LOG(LogPRAISoldier, Warning, TEXT("Controller: CoverOwnerPos output is %s"), *CoverRef->GetName())
		const auto PlayerCoordinates = PlayerPos.GetActorPosition();
		UE_LOG(LogPRAIController, Log, TEXT("Controller: Player pos X: %0.2f, Y: %0.2f"), PlayerCoordinates.X, PlayerCoordinates.Y);
		UE_LOG(LogPRAIController, Log, TEXT("Controller: Cover pos was set X: %0.2f, Y: %0.2f"), CoverPos.X, CoverPos.Y);
		BlackboardComp->SetValueAsVector(CoverKeyName, CoverPos);
		return true;
	}
	return false;
	//MoveToLocation(CoverPos);
}

AActor* ASoldierAIController::GetFocusOnActor()
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
