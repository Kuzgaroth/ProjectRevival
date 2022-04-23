// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "AI/Soldier/SoldierAIController.h"
#include "GameFeature/ChangeWorld.h"
#include "Interfaces/ISaveLoader.h"
#include "PRGameModeBase.generated.h"

class USaveGame;
class UPRSaveGame;
class AAIController;

UCLASS()
class PROJECTREVIVAL_API APRGameModeBase : public AGameModeBase, public IISaveLoader
{
	GENERATED_BODY()

public:
	APRGameModeBase();

	FOnMatchStateChangedSignature OnMatchStateChanged;
	
	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	

	FGameData GetGameData() const {return GameData;}
	void RespawnRequest(AController* Controller);
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate) override;
	virtual bool ClearPause() override;
	void GameOver();
	UFUNCTION(BlueprintCallable)
	void SetCurrentWorld(EChangeWorld NewWorld);
	UFUNCTION(BlueprintCallable)
	EChangeWorld GetCurrentWorld() const{return CurrentWorld;}
	void WriteSaveGame(FName CheckpointName);
	void ClearSaveGame();
	virtual UPRSaveGame* GetSaveFromLoader() override;
protected:
	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	FGameData GameData;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<APawn> AIPawnClass;
	
	EChangeWorld CurrentWorld = EChangeWorld::OrdinaryWorld;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
private:
	EMatchState MatchState = EMatchState::WaitingToStart;
	
	void SpawnBots();
	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);
	void LogPlayerInfo();
	void InitCrates();
	
	void SetMatchState(EMatchState State);
	void LoadSaveGame();
	void SaveFinished(const FString&, const int32, bool);
	void LoadFinished(const FString&, const int32, USaveGame*);
	UPROPERTY(Transient)
	APlayerCharacter* PlayerPawn;
	UPROPERTY()
	UPRSaveGame* SaveGame=nullptr;
};
