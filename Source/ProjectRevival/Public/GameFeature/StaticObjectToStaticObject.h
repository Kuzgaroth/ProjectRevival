// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChangeWorld.h"
#include "StaticObjectToStaticObject.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTREVIVAL_API AStaticObjectToStaticObject : public AChangeWorld
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticObjectToStaticObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TEnumAsByte<EChangeWorld> World = OrdinaryWorld;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UStaticMeshComponent* SuperMesh1;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UStaticMeshComponent* SuperMesh2;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UBoxComponent* CollisionComponent1 = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UBoxComponent* CollisionComponent2 = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	void Changing() override;
};
