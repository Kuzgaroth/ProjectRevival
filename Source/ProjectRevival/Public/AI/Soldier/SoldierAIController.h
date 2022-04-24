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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartFiring);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStartEnteringCover, const FVector&, CoverPosition, AActor*, CoverReference);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartExitingCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartCoverSideMoving, float, SideMovementAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBotStateDelegate, const EBotState, BotStateVal);

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
	void SetPlayerPos(const FPlayerPositionData &NewPlayerPos, bool bIsFromCoordinator = false);
	bool GetBIsFiring() const { return bIsFiring; }
	void SetBIsFiring(bool const bCond) { bIsFiring = bCond; }
	bool GetBIsInCover() const { return bIsInCover; }
	void SetBIsInCover(bool const bCond) { bIsInCover = bCond; }
	bool GetBIsSideTurning() const { return bIsSideTurning; }
	void SetBIsSideTurning(bool const bCond) {bIsSideTurning = bCond; }
	bool GetBIsCoverChangeAllowed() const { return bIsCoverChangeAllowed; }
	void SetBIsCoverChangeAllowed(bool const bCond) {bIsCoverChangeAllowed = bCond; }
	bool GetBIsDecisionMakingAllowed() const { return bIsDecisionMakingAllowed; }
	void SetBIsDecisionMakingAllowed(bool const bCond) { bIsDecisionMakingAllowed = bCond; }
	bool GetBIsFiringAllowed() const { return bIsFiringAllowed; }
	void SetBIsFiringAllowed(bool const bCond) { bIsFiringAllowed = bCond; }
	bool GetBIsPlayerInSight() const { return bIsPlayerInSight; }
	void SetBIsPlayerInSight(bool const bCond);
	bool GetBIsPlayerInSightOfAnyBot() const { return bIsPlayerInSightOfAnyBot; }
	void SetBIsPlayerInSightOfAnyBot(bool const bCond) { bIsPlayerInSightOfAnyBot = bCond; }
	bool GetBIsAppearing() const { return bIsAppearing; }
	UFUNCTION(BlueprintCallable)
	void SetBIsAppearing(bool bCond);
	UFUNCTION(BlueprintCallable)
	EBotState GetBotState() const { return BotState; }
	void SetBotState(EBotState const val);
	bool GetBIsPatrolling() const { return bIsPatrolling; }
	void SetBIsPatrolling(bool const bCond) { bIsPatrolling = bCond; }
	bool GetBIsLoosePlayerTimerSet() const { return bIsLoosePlayerTimerSet; }
	void SetBIsLoosePlayerTimerSet(bool const bCond) { bIsLoosePlayerTimerSet = bCond; }

	UPROPERTY(BlueprintAssignable)
	FPlayerPosDelegate PlayerPosDelegate;
	UPROPERTY(BlueprintAssignable)
	FStartFiring StartFiringDelegate;
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
	//Делегат для передачи в павн состояние бота
	FBotStateDelegate BotStateDelegate;
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
	bool FindPatrolPath();
	void FindNextPatrolPoint();
	void StartCoverTimer();
	void OnCoverTimerFired();
	void StartGeneralTimer();
	void OnGeneralTimerFired();
	void StartFireTimer(float cooldownSeconds);
	void OnFireTimerFired();
	void OnLoosePlayerTimerFired();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	EWing BotWing;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UPRSoldierAIPerceptionComponent* PRPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FPlayerPositionData PlayerPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FVector CoverPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	AActor* CoverRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FVector PatrolPathPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	AActor* PatrolPathRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FVector PatrolPointPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	UBoxComponent* PatrolPointRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	float SideMovementAmount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName FocusOnKeyName = "EnemyActor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName CoverPosKeyName = "CoverPos";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName CoverRefKeyName = "CoverRef";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName PatrolPointPosKeyName = "PatrolPointPos";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName WingKeyName = "WingSide";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName BotStateKeyName = "BotState";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Respawn")
	URespawnComponent* RespawnComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	float GeneralCooldownTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	float CoverChangeTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	float PlayerLooseTime;

	EBotState BotState;
	
	FTimerHandle BTCoverTimerHandle;
	FTimerHandle BTGeneralTimerHandle;
	FTimerHandle BTFireTimerHandle;
	FTimerHandle LoosePlayerTimer;
	
	bool bIsFiring;
	bool bIsInCover;
	bool bIsSideTurning;
	bool bIsCoverChangeAllowed;
	bool bIsDecisionMakingAllowed;
	bool bIsFiringAllowed;
	bool bIsPlayerInSight; // Виден ли игрок в поле зрения этого бота
	bool bIsPlayerInSightOfAnyBot; // Виден ли игрок в поле зрения хоть какого-то бота
	bool bIsAppearing;
	bool bIsPatrolling;
	bool bIsLoosePlayerTimerSet;
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	AActor* GetFocusOnActor();
	void SetToIdleState();
	void SetToBattleState();
};
