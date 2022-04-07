// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "PatrolPathActor.generated.h"

UCLASS()
class PROJECTREVIVAL_API APatrolPathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPathActor();
	bool GetBCanBeTaken() const { return PatrolPath.bCanBeTaken; }
	void SetBCanBeTaken(bool const bCond) { PatrolPath.bCanBeTaken = bCond; }
	bool GetBIsLooped() const { return PatrolPath.bIsLooped; }
	bool GetBIsReversed() const { return PatrolPath.bIsReversed; }
	void SetBIsReversed(bool const bCond) { PatrolPath.bIsReversed = bCond; }
	
	bool AddPatrollingBot();
	void DeletePatrollingBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category="Patrolling")
	FPatrolPath PatrolPath;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TTuple<UBoxComponent*, FVector> GetClosestPoint(FVector BotPos) const;
	TTuple<UBoxComponent*, FVector> GetNextPatrolPoint(UBoxComponent* CurrentPointRef, FVector CurrentPointPos);
};
