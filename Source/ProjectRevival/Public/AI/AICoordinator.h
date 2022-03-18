// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "GameFramework/Actor.h"
#include "AICoordinator.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPRCoordinator, All, All)

DECLARE_MULTICAST_DELEGATE_OneParam(OnBotDiedSignature, AAIController*)

UCLASS()
class PROJECTREVIVAL_API AAICoordinator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAICoordinator();
	OnBotDiedSignature OnBotDied;
	
protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* ArenaComponent;
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerComponent;

	UPROPERTY(EditInstanceOnly, Category="Enemies")
	TSubclassOf<AAIController> EnemyControllerClass;
	UPROPERTY(EditInstanceOnly, Category="Enemies")
	TSubclassOf<AAICharacter> EnemyCharacterClass;
	
	
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ProcessBotDeath(AAIController* BotController);
private:
	UPROPERTY()
	TMap<AAIController*, EWing> BotMap;
	
	bool InitSpawn();
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SpawnBotsBySide(TArray<UChildActorComponent*>& SideComponents, TArray<UChildActorComponent*>& AllComponents, int32& NumSide, int32& NumAll, EWing WingSide);
	void SpawnBot(const FTransform& SpawnTransform, EWing WingSide);
};


