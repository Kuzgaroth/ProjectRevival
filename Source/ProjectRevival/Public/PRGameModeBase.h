// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "AI/Soldier/SoldierAIController.h"
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
	

	FGameData GetGameData() const {return GameData;}
	void RespawnRequest(AController* Controller);
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate) override;
	virtual bool ClearPause() override;
	void GameOver();
protected:
	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	FGameData GameData;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<APawn> AIPawnClass;
private:
	EMatchState MatchState = EMatchState::WaitingToStart;
	
	void SpawnBots();
	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);
	void LogPlayerInfo();
	
	void SetMatchState(EMatchState State);
};
