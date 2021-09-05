// Project Revival. All Rights Reserved


#include "PRGameModeBase.h"
#include "Player/BasePlayerController.h"
#include "Player/BaseCharacter.h"
#include "AIController.h"
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

	SpawnBots();
	CreateTeamsInfo();
	
	CurrentRound = 1;
	StartRound();
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

void APRGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
	const auto KillerPlayerState = KillerController ? KillerController->GetPlayerState<APRPlayerState>() : nullptr;
	const auto VictimPlayerState = VictimController ? VictimController->GetPlayerState<APRPlayerState>() : nullptr;
	if (KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}
	StartRespawn(VictimController);
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
	
}

void APRGameModeBase::StartRound()
{
	RoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &APRGameModeBase::GameTimerUpdate, 1.0f, true);
}

void APRGameModeBase::GameTimerUpdate()
{
	if (--RoundCountDown==0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

		if (CurrentRound + 1 <= GameData.RoundsNum)
		{
			UE_LOG(LogGamePrModeBase, Display, TEXT("Round %i/%i ended"), CurrentRound, GameData.RoundsNum);
			++CurrentRound;
			ResetPlayers();
			StartRound();
			UE_LOG(LogGamePrModeBase, Display, TEXT("Round %i/%i starts"), CurrentRound, GameData.RoundsNum);
		}
		else
		{
			GameOver();
			
		}
	}
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
	SetPlayerColor(Controller);
}

void APRGameModeBase::CreateTeamsInfo()
{
	if (!GetWorld()) return;
	int32 TeamId = 1;
	for (auto It = GetWorld()->GetControllerIterator(); It;++It)
	{

		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<APRPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;
		
		PlayerState->SetTeamId(TeamId);
		PlayerState->SetTeamColor(DetermineColorByTeamID(TeamId));
		PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
		SetPlayerColor(Controller);
		
		TeamId = TeamId == 1 ? 2 : 1;
	}
}

FLinearColor APRGameModeBase::DetermineColorByTeamID(int32 TeamId) const
{
	if (TeamId - 1 <GameData.TeamColors.Num())
	{
		
		return GameData.TeamColors[TeamId-1];
	}
	return GameData.DefaultTeamColor;
}

void APRGameModeBase::SetPlayerColor(AController* Controller)
{
	if (!Controller) return;
	const auto Character = Cast<ABaseCharacter>(Controller->GetPawn());

	if (!Character) return;

	const auto PlayerState = Controller->GetPlayerState<APRPlayerState>();
	if (!PlayerState) return;

	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void APRGameModeBase::LogPlayerInfo()
{
	if (!GetWorld()) return;
	for (auto It = GetWorld()->GetControllerIterator(); It;++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<APRPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		PlayerState->LogInfo();
	}
}

void APRGameModeBase::StartRespawn(AController* Controller)
{
	const auto RespawnAvaliable = RoundCountDown>MinRoundTimeForSpawn+GameData.RespawnTime;
	if (!RespawnAvaliable) return;
	
	const auto RespawnComponent = PRUtils::GetCharacterComponent<URespawnComponent>(Controller);
	if (!RespawnComponent) return;

	RespawnComponent->Respawn(GameData.RespawnTime);
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
