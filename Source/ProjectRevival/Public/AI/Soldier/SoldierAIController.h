// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/PRSoldierAIPerceptionComponent.h"
#include "SoldierAIController.generated.h"

// Объявление делегата передачи положения игрока
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerPos, const FVector&, PlayerPosition);

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
	void SetPlayerPos(const FVector &NewPlayerPos) { PlayerPos=NewPlayerPos; }
	bool GetBIsFiring() const { return bIsFiring; }
	void SetBIsFiring(bool bCond) { bIsFiring = false; }

	FPlayerPos StartFiringAtPlayerPos;
	
	void StartFiring();
	// Функция, к которой должен быть привязан делегат класса Character
	void StopFiring();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UPRSoldierAIPerceptionComponent* PRPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FVector PlayerPos;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName FocusOnKeyName = "EnemyActor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Respawn")
	URespawnComponent* RespawnComponent;

	bool bIsFiring;
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

private:
	AActor* GetFocusOnActor();
};
