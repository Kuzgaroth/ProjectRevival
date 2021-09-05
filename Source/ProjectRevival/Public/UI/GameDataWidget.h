// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameDataWidget.generated.h"

class APRGameModeBase;
class APRPlayerState;

UCLASS()
class PROJECTREVIVAL_API UGameDataWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetKillsNum() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetDeathsNum() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetTotalRoundsNum() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetCurrentRoundNum() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetRoundSecondsRemaining() const;
private:
	APRGameModeBase* GetGameMode() const;
	APRPlayerState* GetPlayerState() const;
};
