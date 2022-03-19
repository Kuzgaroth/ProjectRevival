// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Soldier/SoldierAIController.h"
#include "AICharacter.h"
#include "Components/BoxComponent.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "GameFramework/Actor.h"
#include "AICoordinator.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPRCoordinator, All, All)



UCLASS()
class PROJECTREVIVAL_API AAICoordinator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAICoordinator();

	
protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* ArenaComponent;
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerComponent;

	UPROPERTY(EditInstanceOnly, Category="Enemies")
	TSubclassOf<AAIController> EnemyControllerClass;
	UPROPERTY(EditInstanceOnly, Category="Enemies")
	TSubclassOf<AAICharacter> EnemyCharacterClass;
	UPROPERTY(EditAnywhere)
	float PlayerPositionUpdateTime=2.f;
	
	virtual void PostInitializeComponents() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ProcessBotDeath(ASoldierAIController* BotController);
private:
	UPROPERTY()
	TMap<ASoldierAIController*, EWing> BotMap;
	FVector PlayerLocation;
	
	bool InitSpawn();
	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SpawnBotsBySide(TArray<UChildActorComponent*>& SideComponents, TArray<UChildActorComponent*>& AllComponents, int32& NumSide, int32& NumAll, EWing WingSide);
	void SpawnBot(AActor* PlayerStartActor, EWing WingSide);
	AAICharacter* SpawnCharacterForBot(AActor* PlayerStartActor, const FTransform& Transform);
	void ConnectController(ASoldierAIController* BotController, EWing);
	void ReorganizeBots();
	void UpdatePlayerInfoFromBot(FVector PlayerLocation);
};




