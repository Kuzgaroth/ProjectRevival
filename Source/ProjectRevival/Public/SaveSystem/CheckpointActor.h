// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ICheckpointable.h"
#include "CheckpointActor.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTREVIVAL_API ACheckpointActor : public AActor, public IICheckpointable
{
	GENERATED_BODY()
	
public:	
	ACheckpointActor();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerComponent;
	UPROPERTY(EditInstanceOnly)
	UChildActorComponent* PlayerStartComponent;

	UPROPERTY(EditInstanceOnly)
	FName CheckpointName;
	UPROPERTY(EditInstanceOnly)
	bool FirstCheckPoint=false;
public:	
	FORCEINLINE bool IsFirstCheckPoint() const {return FirstCheckPoint;}
	virtual AActor* GetPlayerStartForCheckpoint() override;
	virtual bool HasName(FName Name) override;
	virtual bool IsFirstCheckpointOnMap() override;
private:
	UFUNCTION()
	void OnTriggerSave(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
