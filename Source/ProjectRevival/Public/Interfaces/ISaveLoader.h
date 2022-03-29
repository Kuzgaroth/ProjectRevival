// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SaveSystem/PRSaveGame.h"
#include "UObject/Interface.h"
#include "ISaveLoader.generated.h"


UINTERFACE(MinimalAPI)
class UISaveLoader : public UInterface
{
	GENERATED_BODY()
};



class PROJECTREVIVAL_API IISaveLoader
{
	GENERATED_BODY()
public:
	virtual UPRSaveGame* GetSaveFromLoader();
};
