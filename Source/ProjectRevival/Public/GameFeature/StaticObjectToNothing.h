// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticObjectToNothing.generated.h"

class UBoxComponent;

UENUM(BlueprintType, Category = "GameRules")
enum EChangeWorld
{
	OrdinaryWorld UMETA(DisplayName = "Ordinary World"),
	OtherWorld UMETA(DisplayName = "Other World")
};

UCLASS()

class PROJECTREVIVAL_API AStaticObjectToNothing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticObjectToNothing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UStaticMeshComponent* SuperMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TEnumAsByte<EChangeWorld> World = OrdinaryWorld;
	
	TEnumAsByte<EChangeWorld> CurrentWorld = OrdinaryWorld;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UBoxComponent* CollisionComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	void Changing();
};
