// Project Revival. All Rights Reserved


#include "PRGameModeBase.h"

#include "AICoordinator.h"
#include "Player/BasePlayerController.h"
#include "Player/BaseCharacter.h"
#include "PRPlayerState.h"
#include "UI/GameHUD.h"
#include "PRUtils.h"
#include "EngineUtils.h"
#include "HealthComponent.h"
#include "WeaponComponent.h"
#include "Interfaces/ICheckpointable.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/PRSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

DEFINE_LOG_CATEGORY_STATIC(LogGamePrModeBase, All, All);
DEFINE_LOG_CATEGORY(LogPRSaveSystem)

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

void APRGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
	if (!SaveGame) return;
	TArray<AActor*> PassedCoordinators;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICoordinator::StaticClass(), PassedCoordinators);
	for (auto CheckpointName: SaveGame->ReachedCheckpoints)
	{
		auto PendingCoordinators = PassedCoordinators.FilterByPredicate([&](AActor* Actor)
		{
			return Actor->ActorHasTag(CheckpointName.CheckpointName);
		});
		PassedCoordinators.RemoveAll([&](AActor* Actor)
		{
			return Actor->ActorHasTag(CheckpointName.CheckpointName);
		});
		for (AActor* PendingCoordinator : PendingCoordinators) PendingCoordinator->Destroy(true);
	}
}

void APRGameModeBase::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}
	
	TArray<AActor*> Checkpoints; /*One actually*/
	AActor* PlayerStart;
	FName CheckpointName;
	if (SaveGame)
	{
		CheckpointName = SaveGame->PlayerSaveData.LastCheckpointReached.CheckpointName;
	}
	else
	{
		CheckpointName = FirstCheckpointName;
	}
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),CheckpointName ,Checkpoints);
	if (Checkpoints.Num()<1) return;
	auto CheckpointInterface = Cast<IICheckpointable>(Checkpoints[0]);
	if (!CheckpointInterface) return;
	PlayerStart = CheckpointInterface->GetPlayerStartForCheckpoint();
	
	RestartPlayerAtPlayerStart(NewPlayer, PlayerStart);
	PlayerPawn = Cast<APlayerCharacter>(NewPlayer->GetPawn());
	Checkpoints[0]->Destroy(true);
	if (SaveGame)
	{
		PlayerPawn->GetHealthComponent()->SetHealth(SaveGame->PlayerSaveData.HP);
		/*for (FWeaponSaveData WeaponSaveData : SaveGame->PlayerSaveData.WeaponSaveDatas)
		{
			FMemoryReader MemReader(SaveGame->PlayerSaveData.WeaponSaveDatas[0].ByteArray);
			FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
			Ar.ArIsSaveGame = true;
			FAmmoData::StaticStruct()->Serialize(Ar);
		}*/
		
		FAmmoData NewAmmoData;
		NewAmmoData.Bullets = SaveGame->PlayerSaveData.WeaponSaveDatas[0].CurrentAmmo;
		NewAmmoData.Clips = SaveGame->PlayerSaveData.WeaponSaveDatas[0].CurrentClips;
		NewAmmoData.bInfiniteAmmo=false;
		PlayerPawn->GetWeaponComponent()->SetWeponData(NewAmmoData);
		
	}
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

void APRGameModeBase::SetCurrentWorld(EChangeWorld NewWorld)
{
	CurrentWorld = NewWorld;
}

void APRGameModeBase::WriteSaveGame(FName CheckpointName)
{
	if (!SaveGame) SaveGame = Cast<UPRSaveGame>(UGameplayStatics::CreateSaveGameObject(UPRSaveGame::StaticClass()));
	
	const auto AsyncDelegate = FAsyncSaveGameToSlotDelegate::CreateUObject(this, &APRGameModeBase::SaveFinished);
	FPlayerSaveData PlayerSaveData;
	auto WeaponsArray = PlayerPawn->GetPlayerWeapons();
	for (const auto Data : WeaponsArray)
		PlayerSaveData.WeaponSaveDatas.Add(FWeaponSaveData(Data.Bullets, Data.Clips));
	PlayerSaveData.HP = PlayerPawn->GetHealthComponent()->GetHealth();
	PlayerSaveData.LastCheckpointReached = FCheckPointSaveData(CheckpointName);
	SaveGame->PlayerSaveData = PlayerSaveData;
	SaveGame->ReachedCheckpoints.Add(FCheckPointSaveData(CheckpointName));
	UGameplayStatics::AsyncSaveGameToSlot(SaveGame, "SaveSlot", 0, AsyncDelegate);
}

void APRGameModeBase::ClearSaveGame()
{
	UGameplayStatics::DeleteGameInSlot("SaveSlot",0);
}

void APRGameModeBase::SetMatchState(EMatchState State)
{
	if (this->MatchState==State) return;
	this->MatchState = State;

	OnMatchStateChanged.Broadcast(this->MatchState);
}

void APRGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist("SaveSlot",0))
		SaveGame = Cast<UPRSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveSlot",0));
}

void APRGameModeBase::SaveFinished(const FString& SlotName, const int32 UserIndex, bool SaveResult)
{
	
	//Logic after save is finished
}

void APRGameModeBase::LoadFinished(const FString& SlotName, const int32 UserIndex, USaveGame* SaveObject)
{
	//Logic after load is finished
}
