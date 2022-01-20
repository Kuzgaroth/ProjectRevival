// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChangeWorld.generated.h"

UENUM(BlueprintType, Category = "GameRules")
enum EChangeWorld
{
	OrdinaryWorld UMETA(DisplayName = "Ordinary World"),
	OtherWorld UMETA(DisplayName = "Other World")
};

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

	TEnumAsByte<EChangeWorld> CurrentWorld = OrdinaryWorld;
	
    virtual void Changing();
};
