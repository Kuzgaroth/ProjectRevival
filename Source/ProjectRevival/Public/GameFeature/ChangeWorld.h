// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "ChangeWorld.generated.h"

UCLASS()
class PROJECTREVIVAL_API AChangeWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChangeWorld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
