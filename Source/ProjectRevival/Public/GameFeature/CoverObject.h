// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "CoverObject.generated.h"

UCLASS()
class PROJECTREVIVAL_API ACoverObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoverObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Занято ли укрытие
	bool bIsTaken;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool IsCoverTaken() const { return bIsTaken; }
};
