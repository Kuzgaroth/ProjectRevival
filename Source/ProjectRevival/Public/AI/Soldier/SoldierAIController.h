// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/PRSoldierAIPerceptionComponent.h"
#include "Components/RespawnComponent.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "SoldierAIController.generated.h"

class ASoldierAIController;

// Объявление делегата передачи положения игрока
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerPosDelegate, const FPlayerPositionData&, PlayerPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStartEnteringCover, const FVector&, CoverPosition, AActor*, CoverReference);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartExitingCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartCoverSideMoving, float, SideMovementAmount);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBotDiedSignature, ASoldierAIController*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerSpottedSignature, FPlayerPositionData)
DECLARE_DELEGATE_RetVal(bool, FOnWingBotsDecision)

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

	FPlayerPositionData GetPlayerPos() const { return PlayerPos; }
	void SetPlayerPos(const FPlayerPositionData &NewPlayerPos);
	bool GetBIsFiring() const { return bIsFiring; }
	void SetBIsFiring(bool const bCond) { bIsFiring = bCond; }
	bool GetBIsInCover() const { return bIsInCover; }
	void SetBIsInCover(bool const bCond) { bIsInCover = bCond; }
	bool GetBIsSideTurning() const { return bIsSideTurning; }
	void SetBIsSideTurning(bool const bCond) {bIsSideTurning = bCond; }

	FPlayerPosDelegate PlayerPosDelegate;
	UPROPERTY(BlueprintAssignable)
	FStartEnteringCover StartEnteringCoverDelegate;
	UPROPERTY(BlueprintAssignable)
	FStartExitingCover StartExitingCoverDelegate;
	UPROPERTY(BlueprintAssignable)
	FStartCoverSideMoving StartCoverSideMovingDelegate;
	//Делегат для оповещения о смерти (Подключен)
	FOnBotDiedSignature OnBotDied;
	//Делегат для оповещения координатора о положении игрока (Подключен)
	FOnPlayerSpottedSignature OnPlayerSpotted;
	//Делегат для выбора действий ботов в крыльях (надо подключить к BehaviorTree)
	FOnWingBotsDecision OnBotWingDecision;
	void StartFiring();
	// Функция, к которой должен быть привязан делегат класса Character
	UFUNCTION()
	void StopFiring();
	void StartEnteringCover();
	UFUNCTION()
	void StopEnteringCover();
	void StartExitingCover();
	UFUNCTION()
	void StopExitingCover();
	void StartCoverSideMoving();
	UFUNCTION()
	void StopCoverSideMoving();
	bool FindNewCover();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	EWing BotWing;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UPRSoldierAIPerceptionComponent* PRPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FPlayerPositionData PlayerPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FVector CoverPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	AActor* CoverRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	float SideMovementAmount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName FocusOnKeyName = "EnemyActor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName CoverKeyName = "CoverPos";

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

