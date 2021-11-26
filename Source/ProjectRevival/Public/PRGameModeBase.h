// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "AIController.h"
#include "PRGameModeBase.generated.h"


class AAIController;

UCLASS()
class PROJECTREVIVAL_API APRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APRGameModeBase();

	FOnMatchStateChangedSignature OnMatchStateChanged;
	
	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	void Killed(AController* KillerController, AController* VictimController);

	FGameData GetGameData() const {return GameData;}
	int32 GetCurrentRoundNum() const {return CurrentRound;}
	int32 GetRoundSecondsRemaining() const {return RoundCountDown;}
	void RespawnRequest(AController* Controller);
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate) override;
	virtual bool ClearPause() override;
protected:
	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	FGameData GameData;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<APawn> AIPawnClass;
private:
	EMatchState MatchState = EMatchState::WaitingToStart;
	int32 CurrentRound;
	int32 RoundCountDown;
	FTimerHandle GameRoundTimerHandle;
	
	void SpawnBots();
	void StartRound();
	void GameTimerUpdate();
	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);
	void CreateTeamsInfo();
	FLinearColor DetermineColorByTeamID(int32 TeamId) const;
	void SetPlayerColor(AController* Controller);

	void LogPlayerInfo();
	void StartRespawn(AController* Controller);
	void GameOver();
	void SetMatchState(EMatchState State);
};
