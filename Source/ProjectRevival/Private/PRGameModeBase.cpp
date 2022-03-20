// Project Revival. All Rights Reserved


#include "PRGameModeBase.h"
#include "Player/BasePlayerController.h"
#include "Player/BaseCharacter.h"
#include "PRPlayerState.h"
#include "UI/GameHUD.h"
#include "RespawnComponent.h"
#include "PRUtils.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGamePrModeBase, All, All);

constexpr static int32 MinRoundTimeForSpawn = 10;

APRGameModeBase::APRGameModeBase()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
	HUDClass = AGameHUD::StaticClass();
	PlayerStateClass = APRPlayerState::StaticClass();
}

void APRGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	//SpawnBots();
	
	SetMatchState(EMatchState::InProgress);
}

UClass* APRGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void APRGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

bool APRGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);;
	if (PauseSet)
	{
		SetMatchState(EMatchState::Pause);
	}
	return PauseSet;
}

bool APRGameModeBase::ClearPause()
{
	const bool PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		SetMatchState(EMatchState::InProgress);
	}
	return PauseCleared;
}

void APRGameModeBase::SpawnBots()
{
	if (!GetWorld()) return;

	for (int32 i=0;i<GameData.PlayersNum-1;++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const auto AIPRController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		
		RestartPlayer(AIPRController);
	}
	ResetPlayers();
}

void APRGameModeBase::ResetPlayers()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It;++It)
	{
		ResetOnePlayer(It->Get());
	}
}

void APRGameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}
	RestartPlayer(Controller);
}


void APRGameModeBase::LogPlayerInfo()
{
	
}

void APRGameModeBase::GameOver()
{
	UE_LOG(LogGamePrModeBase, Display, TEXT("======= GAME OVER ==========="))
	LogPlayerInfo();

	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}
	SetMatchState(EMatchState::GameOver);
}

void APRGameModeBase::SetMatchState(EMatchState State)
{
	if (this->MatchState==State) return;
	this->MatchState = State;

	OnMatchStateChanged.Broadcast(this->MatchState);
}
