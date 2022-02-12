// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "EnvironmentQuery/EQSTestingPawn.h"
#include "Soldier/SoldierAIController.h"
#include "SoldierEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartEnteringCoverForAnim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartExitingCoverForAnim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartCoverSideMovingForAnim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopEnteringCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopExitingCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopCoverSideMoving);

UCLASS()
class PROJECTREVIVAL_API ASoldierEnemy : public AAICharacter, public IICoverable
{
	GENERATED_BODY()
public:
	
	virtual void OnDeath() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStartEnteringCoverForAnim StartEnteringCoverForAnimDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStartExitingCoverForAnim StartExitingCoverForAnimDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStartCoverSideMovingForAnim StartCoverSideMovingForAnimDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopEnteringCover StopEnteringCoverDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopExitingCover StopExitingCoverDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopCoverSideMoving StopCoverSideMovingDelegate;

	virtual ECoverType CheckCover() override;
	
	UFUNCTION(BlueprintCallable)
	FCoverData& GetCoverData();

	UFUNCTION()
	void StartCoverSoldier(const FVector& CoverPos);

	UFUNCTION(BlueprintCallable)
	void StartCoverSoldierFinish();

	UFUNCTION()
	void StopCoverSoldier();

	UFUNCTION(BlueprintCallable)
	void StopCoverSoldierFinish();

	UFUNCTION()
	void ChangeCoverSide(float Amount);

	UFUNCTION(BlueprintCallable)
	void ChangeCoverSideFinish();

	UPROPERTY(BlueprintReadOnly)
	bool bIsInCoverBP;

	UPROPERTY(BlueprintReadOnly)
	FCoverData CoverData;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float CurrentHealth, float HealthDelta) override;

	virtual TEnumAsByte<ECoverSide> CheckSideByNormal(FVector Forward, FVector Normal);
	virtual TEnumAsByte<ECoverPart> GetCoverPart(int8 PartPos);

	float SideMoveAmount;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void UpdateHealthWidgetVisibility();
	void UpdateHStateBlackboardKey(uint8 EnumKey);
	void CoverCrouch();

	// void MoveForward(float Amount);
	// void MoveRight(float Amount);
	// void StartRun();
	// void StopRun();
	// virtual bool IsRunning() const override;
	// bool bWantsToRun = false;
	// bool IsMovingForward = false;
};
