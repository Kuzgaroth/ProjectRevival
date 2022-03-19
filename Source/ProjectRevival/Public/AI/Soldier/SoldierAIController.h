// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/PRSoldierAIPerceptionComponent.h"
#include "Components/RespawnComponent.h"
#include "SoldierAIController.generated.h"

class ASoldierAIController;

// Объявление делегата передачи положения игрока
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerPosDelegate, const FVector&, PlayerPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartEnteringCover, const FVector&, CoverPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartExitingCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartCoverSideMoving, float, SideMovementAmount);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBotDiedSignature, ASoldierAIController*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerSpottedSignature, FVector)
DECLARE_DELEGATE_RetVal(bool, FOnWingBotsDecision)

DECLARE_LOG_CATEGORY_EXTERN(LogPRAIController, Log, All);

class UPRAIPerceptionComponent;
class URespawnComponent;

/*
 *   Контроллер бота-солдата
 */
UCLASS()
class PROJECTREVIVAL_API ASoldierAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASoldierAIController();

	FVector GetPlayerPos() const { return PlayerPos; }
	void SetPlayerPos(const FVector &NewPlayerPos);
	bool GetBIsFiring() const { return bIsFiring; }
	void SetBIsFiring(bool bCond) { bIsFiring = bCond; }
	bool GetBIsInCover() const { return bIsInCover; }
	void SetBIsInCover(bool bCond) { bIsInCover = bCond; }

	FPlayerPosDelegate PlayerPosDelegate;
	UPROPERTY(BlueprintAssignable)
	FStartEnteringCover StartEnteringCoverDelegate;
	UPROPERTY(BlueprintAssignable)
	FStartExitingCover StartExitingCoverDelegate;
	UPROPERTY(BlueprintAssignable)
	FStartCoverSideMoving StartCoverSideMovingDelegate;
	FOnBotDiedSignature OnBotDied;
	FOnPlayerSpottedSignature OnPlayerSpotted;
	FOnWingBotsDecision OnBotWingDecision;
	void StartFiring();
	// Функция, к которой должен быть привязан делегат класса Character
	void StopFiring();
	void StartEnteringCover();
	void StopEnteringCover();
	void StartExitingCover();
	void StopExitingCover();
	void StartCoverSideMoving();
	void StopCoverSideMoving();
	void FindNewCover();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	EWing BotWing;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UPRSoldierAIPerceptionComponent* PRPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FVector PlayerPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FVector CoverPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	float SideMovementAmount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName FocusOnKeyName = "EnemyActor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName CoverKeyname = "CoverPos";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Respawn")
	URespawnComponent* RespawnComponent;
	
	bool bIsFiring;
	bool bIsInCover;
	bool bIsSideTurning;
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	AActor* GetFocusOnActor();
};

