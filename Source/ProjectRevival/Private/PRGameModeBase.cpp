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
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

DEFINE_LOG_CATEGORY_STATIC(LogGamePrModeBase, All, All);


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
	UE_LOG(LogPRSaveSystem, Display, TEXT("----Destroying coordinators----"))
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
		for (AActor* PendingCoordinator : PendingCoordinators)
		{
			UE_LOG(LogPRSaveSystem, Display, TEXT("Destroying %s"), *PendingCoordinator->GetName())
			PendingCoordinator->Destroy(true);
		}
	}
	
	TArray<AActor*> Checkpoints; 
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UICheckpointable::StaticClass(),Checkpoints);
	UE_LOG(LogPRSaveSystem, Display, TEXT("----Destroying reached checkpoints----"))
	for (int32 It=0;It<SaveGame->ReachedCheckpoints.Num();It++)
	{
		if (SaveGame->ReachedCheckpoints[It].CheckpointName==SaveGame->PlayerSaveData.LastCheckpointReached.CheckpointName) continue;;
		const auto Checkpoint = Checkpoints.FindByPredicate([&](AActor* Item)
		{
			const auto ItemInterface = Cast<IICheckpointable>(Item);
			return ItemInterface && ItemInterface->HasName(SaveGame->ReachedCheckpoints[It].CheckpointName);
		});
		UE_LOG(LogPRSaveSystem, Display, TEXT("Destroying %s"), *(*Checkpoint)->GetName())
		(*Checkpoint)->Destroy(true);
	}
	InitCrates();
}
void APRGameModeBase::InitCrates()
{
	for (TActorIterator<AAmmoCrate> It(GetWorld()); It; ++It)
	{
		AAmmoCrate* Crate = Cast<AAmmoCrate>(*It);
		Crate->SetCurrentClipsAmount(SaveGame->AmmoCrates.Find(*Crate->GetName())->CurrentClips);
	}
}

void APRGameModeBase::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}
	UE_LOG(LogPRSaveSystem, Display, TEXT("----Spawning player at checkpoint----"))
	TArray<AActor*> Checkpoints; /*One actually*/
	FName CheckpointName;
	if (SaveGame)
	{
		UE_LOG(LogPRSaveSystem, Display, TEXT("----Spawning player at particular checkpoint----"))
		CheckpointName = SaveGame->PlayerSaveData.LastCheckpointReached.CheckpointName;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(),CheckpointName ,Checkpoints);
		InitCrates();
	}
	else
	{
		UE_LOG(LogPRSaveSystem, Display, TEXT("----Spawning player at first checkpoint----"))
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UICheckpointable::StaticClass(),Checkpoints);
		Checkpoints.FilterByPredicate([](AActor* Checkpoint)
		{
			const auto CheckpointInterface = Cast<IICheckpointable>(Checkpoint);
			return CheckpointInterface && CheckpointInterface->IsFirstCheckpointOnMap();
		});
	}
	
	if (Checkpoints.Num()<1)
	{
		UE_LOG(LogPRSaveSystem, Display, TEXT("----No checkpoints----"))
		return;
	}
	const auto CheckpointInterface = Cast<IICheckpointable>(Checkpoints[0]);
	if (!CheckpointInterface)
	{
		UE_LOG(LogPRSaveSystem, Display, TEXT("----No interface in checkpoint----"))
		return;
	}
	
	AActor* PlayerStart = CheckpointInterface->GetPlayerStartForCheckpoint();
	UE_LOG(LogPRSaveSystem, Display, TEXT("---Checkpoint to spawn: %s----"), *PlayerStart->GetHumanReadableName());
	
	RestartPlayerAtPlayerStart(NewPlayer, PlayerStart);
	Checkpoints[0]->Destroy(true);
	PlayerPawn = Cast<APlayerCharacter>(NewPlayer->GetPawn());
	UE_LOG(LogPRSaveSystem, Display, TEXT("----Player spawn process ended----"));
	/*if (SaveGame)
	{
		UE_LOG(LogPRSaveSystem, Display, TEXT("----Loading player save data----"))
		PlayerPawn->GetHealthComponent()->SetHealth(SaveGame->PlayerSaveData.HP);
		for (FWeaponSaveData WeaponSaveData : SaveGame->PlayerSaveData.WeaponSaveDatas)
		{
			FMemoryReader MemReader(SaveGame->PlayerSaveData.WeaponSaveDatas[0].ByteArray);
			FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
			Ar.ArIsSaveGame = true;
			FAmmoData::StaticStruct()->Serialize(Ar);
		}
		
		FAmmoData NewAmmoData;
		NewAmmoData.Bullets = SaveGame->PlayerSaveData.WeaponSaveDatas[0].CurrentAmmo;
		NewAmmoData.Clips = SaveGame->PlayerSaveData.WeaponSaveDatas[0].CurrentClips;
		NewAmmoData.bInfiniteAmmo=false;
		PlayerPawn->GetWeaponComponent()->SetWeponData(NewAmmoData);
		
	}
	UE_LOG(LogPRSaveSystem, Display, TEXT("----Load process ended----"))*/
}

APawn* APRGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
	APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
	if (!ResultPawn)
	{
		UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
	}
	return ResultPawn;
}

void APRGameModeBase::ResetOnePlayer(AController* Controller)
{
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
}

void APRGameModeBase::SetCurrentWorld(EChangeWorld NewWorld)
{
	CurrentWorld = NewWorld;
}

void APRGameModeBase::WriteSaveGame(FName CheckpointName)
{
	UE_LOG(LogPRSaveSystem, Display, TEXT("---- Save process started ----"))
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
	for (TActorIterator<AAmmoCrate> It(GetWorld()); It; ++It)
	{
		SaveGame->AmmoCrates.Add(*It->GetName(),FAmmoCrateSaveData(It->GetCurrentClipsAmount()));
		//UE_LOG(LogPRSaveSystem, Display, TEXT("SaveGame->AmmoCrates.Add(%s)"), *It->GetName());
	}
	UGameplayStatics::AsyncSaveGameToSlot(SaveGame, "SaveSlot", 0, AsyncDelegate);
}

void APRGameModeBase::ClearSaveGame()
{
	UGameplayStatics::DeleteGameInSlot("SaveSlot",0);
}

UPRSaveGame* APRGameModeBase::GetSaveFromLoader()
{
	return SaveGame;
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
	UE_LOG(LogPRSaveSystem, Display, TEXT("----Save process finished----"))
	//Logic after save is finished
}

void APRGameModeBase::LoadFinished(const FString& SlotName, const int32 UserIndex, USaveGame* SaveObject)
{
	//Logic after load is finished
}
