// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "CourtLairGameMode.generated.h"



UCLASS()
class PROJECTREVIVAL_API ACourtLairGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACourtLairGameMode();

	FOnMatchStateChangedSignature OnMatchStateChanged;
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate) override;
	virtual bool ClearPause() override;
protected:
	void SetMatchState(EMatchState NewMatchState);
	virtual void BeginPlay() override;
	
private:
	EMatchState MatchState = EMatchState::WaitingToStart;
};
