// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "CutsceneTriggerActor.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTREVIVAL_API ACutsceneTriggerActor : public AActor
{
	GENERATED_BODY()

public:
	ACutsceneTriggerActor();
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, Category="Components")
	UBoxComponent* CollisionComponent;
	UPROPERTY(EditAnywhere, Category="LevelSequence")
	ULevelSequence* SequenceAsset;
	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;
	UPROPERTY()
	ULevelSequencePlayer* SequencePlayer;
private:
	UFUNCTION()
	void OnSequencePlay();
	UFUNCTION()
	void OnSequenceEnd();
};


