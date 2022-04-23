// Project Revival. All Rights Reserved


#include "AI/Soldier/SoldierAIController.h"
#include "AI/AICharacter.h"
#include "BasePickup.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RespawnComponent.h"
#include "SoldierEnemy.h"
#include "Components/BoxComponent.h"
#include "GameFeature/PatrolPathActor.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

DEFINE_LOG_CATEGORY(LogPRAIController);
DEFINE_LOG_CATEGORY(LogPRAIDecorators);
DEFINE_LOG_CATEGORY(LogPRAITasks);
DEFINE_LOG_CATEGORY(LogPRAIServices);

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
	bIsCoverChangeAllowed = true;
	bIsDecisionMakingAllowed = true;
	bIsFiringAllowed = true;
	bIsAppearing = true;
	bIsPatrolling = false;
	bIsLoosePlayerTimerSet = false;
	PlayerLooseTime = 10.0f;
}

void ASoldierAIController::SetPlayerPos(const FPlayerPositionData& NewPlayerPos, bool bIsFromCoordinator)
{
	if (NewPlayerPos.GetActor())
	{
		if (bIsFromCoordinator)
		{
			UE_LOG(LogPRAIController, Log, TEXT("From Coordinator"))
			SetBIsPlayerInSight(true);
			if (GetBIsLoosePlayerTimerSet())
			{
				GetWorld()->GetTimerManager().ClearTimer(LoosePlayerTimer);
				SetBIsLoosePlayerTimerSet(false);
			}
		}
		UE_LOG(LogPRAIController, Log, TEXT("Player Pos: X %0.2f, Y %0.2f, Ticks %lld"), PlayerPos.GetActorPosition().X,
			PlayerPos.GetActorPosition().Y, PlayerPos.GetInfoUpdateTime().GetTicks())
		PlayerPos.SetActor(NewPlayerPos.GetActor());
		PlayerPos.SetInfoUpdateTime(NewPlayerPos.GetInfoUpdateTime().GetTicks());
	}
	if (NewPlayerPos.GetCover())
	{
		PlayerPos.SetCover(NewPlayerPos.GetCover());
	}
	PlayerPosDelegate.Broadcast(PlayerPos);
	OnPlayerSpotted.Broadcast(PlayerPos);
}

void ASoldierAIController::SetBIsPlayerInSight(bool const bCond)
{
	bIsPlayerInSight = bCond;
	UE_LOG(LogPRAIController, Log, TEXT("bIsPlayerInSight set to %s"), bIsPlayerInSight?TEXT("true"):TEXT("false"))
}

void ASoldierAIController::SetBIsAppearing(bool bCond)
{
	bIsAppearing = bCond;
}

void ASoldierAIController::SetBotState(EBotState const val)
{
	BotState = val;
	BlackboardComponent->SetValueAsInt(BotStateKeyName, uint8(BotState));
	BotStateDelegate.Broadcast(GetBotState());
}

void ASoldierAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto AIChar = Cast<AAICharacter>(InPawn);
	if (AIChar)
	{
		RunBehaviorTree(AIChar->BehaviorTreeAsset);
		BlackboardComponent = GetBlackboardComponent();
		//SetBotState(EBotState::Idle);
		auto Actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
		Actor = Cast<APlayerCharacter>(Actor);
		if (Actor)
		{
			const FPlayerPositionData ActorPos(Actor, nullptr);
			SetPlayerPos(ActorPos);
		}
		SetBotState(EBotState::Battle);
		Cast<ASoldierEnemy>(GetPawn())->StopEnteringCoverDelegate.AddDynamic(this, &ASoldierAIController::StopEnteringCover);
		Cast<ASoldierEnemy>(GetPawn())->StopExitingCoverDelegate.AddDynamic(this, &ASoldierAIController::StopExitingCover);
		Cast<ASoldierEnemy>(GetPawn())->StopCoverSideMovingDelegate.AddDynamic(this, &ASoldierAIController::StopCoverSideMoving);
		Cast<ASoldierEnemy>(GetPawn())->StopFireDelegate.AddDynamic(this, &ASoldierAIController::StopFiring);
		Cast<ASoldierEnemy>(GetPawn())->SoldierWorldChangeDelegate.AddDynamic(this, &ASoldierAIController::SetBIsAppearing);
		if (BlackboardComponent) BlackboardComponent->SetValueAsEnum(WingKeyName, uint8(BotWing));
	}
}

void ASoldierAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*if (bIsPlayerInSight && bIsAppearing)
	{
		SetToBattleState();
	}
	else
	{
		if (!GetBIsLoosePlayerTimerSet())
		{
			SetBIsLoosePlayerTimerSet(true);
			//TODO Добавить сброс таймера, если приходит не мусорные данные из координатора или персепшона
			GetWorld()->GetTimerManager().SetTimer(LoosePlayerTimer, this, &ASoldierAIController::OnLoosePlayerTimerFired,
											   PlayerLooseTime, false, -1);
		}
	}
	if (!bIsAppearing) SetToIdleState();*/
	
	//const auto AimActor = GetFocusOnActor();
	//SetFocus(AimActor);
	//SetBotState(EBotState::Battle);
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
	const FVector PlayerCoordinates = PlayerPos.GetActorPosition();
	UE_LOG(LogPRAIController, Log, TEXT("Shoot at Player pos X: %0.2f, Y: %0.2f"), PlayerCoordinates.X, PlayerCoordinates.Y)
	StartFiringDelegate.Broadcast();
	SetBIsFiring(true);
}

void ASoldierAIController::StopFiring()
{
	UE_LOG(LogPRAIController, Log, TEXT("StopFiring() was called"))
	SetBIsFiring(false);
}

void ASoldierAIController::StartEnteringCover()
{
	UE_LOG(LogPRAIController, Log, TEXT("StartEnteringCover() was called"))
	UE_LOG(LogPRAIController, Log, TEXT("%i Cover pos X: %0.2f, Y: %0.2f"), BotWing, CoverPos.X, CoverPos.Y)
	StartEnteringCoverDelegate.Broadcast(CoverPos, CoverRef);
}

void ASoldierAIController::StopEnteringCover()
{
	UE_LOG(LogPRAIController, Log, TEXT("StopEnteringCover() was called"))
	SetBIsInCover(true);
}

void ASoldierAIController::StartExitingCover()
{
	UE_LOG(LogPRAIController, Log, TEXT("StartExitingCover() was called"))
	StartExitingCoverDelegate.Broadcast();
}

void ASoldierAIController::StopExitingCover()
{
	UE_LOG(LogPRAIController, Log, TEXT("StopExitingCover() was called"))
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
	bool const bFlag = PRPerceptionComponent->GetBestCoverWing(BotWing, CoverPos, CoverRef);
	if (bFlag && BlackboardComponent)
	{
		const auto PlayerCoordinates = PlayerPos.GetActorPosition();
		UE_LOG(LogPRAIController, Log, TEXT("Controller: Player pos X: %0.2f, Y: %0.2f"), PlayerCoordinates.X,
		       PlayerCoordinates.Y)
		UE_LOG(LogPRAIController, Log, TEXT("Controller: Cover pos was set X: %0.2f, Y: %0.2f"), CoverPos.X, CoverPos.Y)
		BlackboardComponent->SetValueAsVector(CoverPosKeyName, CoverPos);
		BlackboardComponent->SetValueAsObject(CoverRefKeyName, CoverRef);
		return true;
	}
	return false;
}

bool ASoldierAIController::FindPatrolPath()
{
	UE_LOG(LogPRAIController, Warning, TEXT("Searching for patrol path"))
	const bool bIsThereFreePatrolPath = PRPerceptionComponent->GetBestPatrollingPath(PatrolPathPos, PatrolPathRef);
	if (bIsThereFreePatrolPath)
	{
		SetBIsPatrolling(true);
		const auto PatrolPath = Cast<APatrolPathActor>(PatrolPathRef);
		const TTuple<UBoxComponent*, FVector> Pair = PatrolPath->GetClosestPoint(GetPawn()->GetActorLocation());
		UE_LOG(LogPRAIController, Warning, TEXT("%s %0.2f %0.2f"), *FString(Pair.Key->GetName()), Pair.Value.X, Pair.Value.Y)
		UE_LOG(LogPRAIController, Warning, TEXT("%s"), *FString(PatrolPathRef->GetName()))
		PatrolPointRef = Pair.Key;
		PatrolPointPos = Pair.Value;

		UE_LOG(LogPRAIController, Warning, TEXT("Updating value in BB"))
		BlackboardComponent->SetValueAsVector(PatrolPointPosKeyName, PatrolPointPos);
		UE_LOG(LogPRAIController, Warning, TEXT("Adding patrol bot"))
		PatrolPath->AddPatrollingBot();
		// if (bIsInCover)
		// {
		// 	StartExitingCover();
		// }
		// else if (bIsFiring)
		// {
		// 	Cast<ASoldierEnemy>(GetPawn())->StopFiringImmediately();
		// }
		return true;
	}
	return false;
}

void ASoldierAIController::FindNextPatrolPoint()
{
	UE_LOG(LogPRAIController, Warning, TEXT("Searching for patrol point"))
	const TTuple<UBoxComponent*, FVector> Pair = Cast<APatrolPathActor>(PatrolPathRef)->GetNextPatrolPoint(PatrolPointRef, PatrolPointPos);
	PatrolPointRef = Pair.Key;
	PatrolPointPos = Pair.Value;

	BlackboardComponent->SetValueAsVector(PatrolPointPosKeyName, PatrolPointPos);
}

void ASoldierAIController::StartCoverTimer()
{
	SetBIsCoverChangeAllowed(false);
	GetWorld()->GetTimerManager().SetTimer(BTCoverTimerHandle, this, &ASoldierAIController::OnCoverTimerFired, 10.0f, false, -1);
	UE_LOG(LogPRAIController, Log, TEXT("Cover Change Cooldown Started"))
}

void ASoldierAIController::OnCoverTimerFired()
{
	SetBIsCoverChangeAllowed(true);
	GetWorld()->GetTimerManager().ClearTimer(BTCoverTimerHandle);
	UE_LOG(LogPRAIController, Log, TEXT("Cover Change Cooldown Ended"))
}

void ASoldierAIController::StartGeneralTimer()
{
	if (GetBIsDecisionMakingAllowed())
	{
		SetBIsDecisionMakingAllowed(false);
		GetWorld()->GetTimerManager().SetTimer(BTGeneralTimerHandle, this, &ASoldierAIController::OnGeneralTimerFired, 0.5f, false, -1);
		UE_LOG(LogPRAIController, Log, TEXT("Decision making cooldown started"))
	}
}

void ASoldierAIController::OnGeneralTimerFired()
{
	SetBIsDecisionMakingAllowed(true);
	GetWorld()->GetTimerManager().ClearTimer(BTGeneralTimerHandle);
	UE_LOG(LogPRAIController, Log, TEXT("Decision making cooldown ended"))
}

void ASoldierAIController::StartFireTimer(float cooldownSeconds)
{
	if (GetBIsFiringAllowed())
	{
		SetBIsFiringAllowed(false);
		GetWorld()->GetTimerManager().SetTimer(BTFireTimerHandle,this, &ASoldierAIController::OnFireTimerFired, cooldownSeconds, false, -1);
	}
}

void ASoldierAIController::OnFireTimerFired()
{
	SetBIsFiringAllowed(true);
	GetWorld()->GetTimerManager().ClearTimer(BTFireTimerHandle);
}

void ASoldierAIController::OnLoosePlayerTimerFired()
{
	UE_LOG(LogPRAIController, Log, TEXT("OnLoosePlayerTimerFired"))
	SetToIdleState();
	SetBIsLoosePlayerTimerSet(false);
	GetWorld()->GetTimerManager().ClearTimer(LoosePlayerTimer);
}

AActor* ASoldierAIController::GetFocusOnActor()
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}

void ASoldierAIController::SetToIdleState()
{
	SetBotState(EBotState::Idle);
	UE_LOG(LogPRAIController, Log, TEXT("State set to Idle"))
	if (GetBIsInCover())
	{
		StartExitingCover();
	}
	GetWorld()->GetTimerManager().ClearTimer(BTCoverTimerHandle);
	SetBIsCoverChangeAllowed(true);
}

void ASoldierAIController::SetToBattleState()
{
	if (GetBIsPatrolling())
	{
		SetBIsPatrolling(false);
		Cast<APatrolPathActor>(PatrolPathRef)->DeletePatrollingBot();
		SetBotState(EBotState::Battle);
		UE_LOG(LogPRAIController, Log, TEXT("State set to Battle"))
	}
}
