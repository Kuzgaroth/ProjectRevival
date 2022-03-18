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

private:
	APRGameModeBase* GetGameMode() const;
	APRPlayerState* GetPlayerState() const;
};
