// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "TickActor.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTickDelegate, float)

UCLASS()
class PROJECTREVIVAL_API ATickActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATickActor();
	
	
protected:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//FTimeline& Timeline;
public:
	FOnTickDelegate OnTick;
	void SetTimeline(FTimeline& Timeline);
	virtual void Tick(float DeltaTime) override;
};
