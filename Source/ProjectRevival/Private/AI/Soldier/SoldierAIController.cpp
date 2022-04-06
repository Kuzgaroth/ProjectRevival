// Project Revival. All Rights Reserved


#include "AI/Soldier/SoldierAIController.h"
#include "AI/AICharacter.h"
#include "BasePickup.h"
#include "PlayerCharacter.h"
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
	UE_LOG(LogPRAIController, Log, TEXT("bIsPlayerInSight: %s"), GetBIsPlayerInSight()?TEXT("true"):TEXT("false"))
	PlayerPosDelegate.Broadcast(PlayerPos);
	OnPlayerSpotted.Broadcast(PlayerPos);
}

void ASoldierAIController::SetBIsAppearing(bool bCond)
{
	bIsAppearing = bCond;
	UE_LOG(LogPRAIController, Warning, TEXT("bIsAppearing is updated in Controller: %s"), bIsAppearing?TEXT("true"):TEXT("false"))
}

void ASoldierAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto AIChar = Cast<AAICharacter>(InPawn);
	if (AIChar)
	{
		auto Actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
		Actor = Cast<APlayerCharacter>(Actor);
		if (Actor)
		{
			const FPlayerPositionData ActorPos(Actor, nullptr);
			SetPlayerPos(ActorPos);
		}
		RunBehaviorTree(AIChar->BehaviorTreeAsset);
		Cast<ASoldierEnemy>(GetPawn())->StopEnteringCoverDelegate.AddDynamic(this, &ASoldierAIController::StopEnteringCover);
		Cast<ASoldierEnemy>(GetPawn())->StopExitingCoverDelegate.AddDynamic(this, &ASoldierAIController::StopExitingCover);
		Cast<ASoldierEnemy>(GetPawn())->StopCoverSideMovingDelegate.AddDynamic(this, &ASoldierAIController::StopCoverSideMoving);
		Cast<ASoldierEnemy>(GetPawn())->StopFireDelegate.AddDynamic(this, &ASoldierAIController::StopFiring);
		Cast<ASoldierEnemy>(GetPawn())->SoldierWorldChangeDelegate.AddDynamic(this, &ASoldierAIController::SetBIsAppearing);
		UE_LOG(LogPRAIController, Warning, TEXT("delegate is bound %s"), Cast<ASoldierEnemy>(GetPawn())->SoldierWorldChangeDelegate.IsBound()?TEXT("true"):TEXT("false"))
		const auto BlackboardComp = GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsEnum(WingKeyName, uint8(BotWing));
		}
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
	const auto BlackboardComp = GetBlackboardComponent();
	if (bFlag && BlackboardComp)
	{
		const auto PlayerCoordinates = PlayerPos.GetActorPosition();
		UE_LOG(LogPRAIController, Log, TEXT("Player pos X: %0.2f, Y: %0.2f"), PlayerCoordinates.X, PlayerCoordinates.Y)
		UE_LOG(LogPRAIController, Log, TEXT("Cover pos was set X: %0.2f, Y: %0.2f"), CoverPos.X, CoverPos.Y)
		BlackboardComp->SetValueAsVector(CoverPosKeyName, CoverPos);
		BlackboardComp->SetValueAsObject(CoverRefKeyName, CoverRef);
		return true;
	}
	return false;
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

AActor* ASoldierAIController::GetFocusOnActor()
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
