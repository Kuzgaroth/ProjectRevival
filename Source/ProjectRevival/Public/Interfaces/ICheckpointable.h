// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICheckpointable.generated.h"


UINTERFACE(MinimalAPI)
class UICheckpointable : public UInterface
{
	GENERATED_BODY()
};


class PROJECTREVIVAL_API IICheckpointable
{
	GENERATED_BODY()

public:
	virtual AActor* GetPlayerStartForCheckpoint();
	virtual bool HasName(FName CheckpointName);
	virtual bool IsFirstCheckpointOnMap();
};
