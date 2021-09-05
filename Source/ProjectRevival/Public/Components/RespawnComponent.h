// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREVIVAL_API URespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URespawnComponent();
	void Respawn(int32 RespawnTime);
	int32 GetRespawnCountdown() const {return RespawnCountdown;}
	bool IsRespawnInProgress() const;
protected:
	virtual void BeginPlay() override;
private:
	FTimerHandle RespawnTimerHandle;
	int32 RespawnCountdown;
	void RespawnTimerUpdate();
		
};


