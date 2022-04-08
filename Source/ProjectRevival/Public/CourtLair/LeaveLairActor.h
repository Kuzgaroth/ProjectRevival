// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LeaveLairActor.generated.h"

UCLASS()
class PROJECTREVIVAL_API ALeaveLairActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALeaveLairActor();

protected:
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* TriggerComponent;
	
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnTriggerLeaveComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									 const FHitResult& SweepResult);
private:
	
};
