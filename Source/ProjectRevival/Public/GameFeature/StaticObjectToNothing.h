// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChangeWorld.h"
#include "StaticObjectToNothing.generated.h"

class UBoxComponent;

UCLASS()

class PROJECTREVIVAL_API AStaticObjectToNothing : public AChangeWorld
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticObjectToNothing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FCollisionResponseContainer CollisionResponseContainer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UStaticMeshComponent* SuperMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TEnumAsByte<EChangeWorld> World = OrdinaryWorld;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UBoxComponent* CollisionComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	
	virtual void TimeLineFinished() override;
	virtual void TimeLineFloatReturn(float Value) override;
	virtual void Changing() override;
};
