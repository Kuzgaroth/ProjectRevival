// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameModeBase.generated.h"



UCLASS()
class PROJECTREVIVAL_API AMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMenuGameModeBase();
	void ClearSaveData();
};
