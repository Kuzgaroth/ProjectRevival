// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "EnvironmentQuery/EQSTestingPawn.h"
#include "Player/BaseCharacter.h"
#include "Soldier/SoldierAIController.h"
#include "SoldierEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopEnteringCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopExitingCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopCoverSideMoving);

class UBehaviorTree;
class UWidgetComponent;

UENUM(BlueprintType)
enum class HealthStateSoldier: uint8
{
	NEAR_DEATH,
	BADLY_INJURED,
	SLIGHTLY_INJURED,
	FULL_HEALTH
};

UCLASS()
class PROJECTREVIVAL_API ASoldierEnemy : public ABaseCharacter, public IICoverable
{
	GENERATED_BODY()
public:
	ASoldierEnemy(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	HealthStateSoldier HState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	ASoldierAIController* AICon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBlackboardComponent* BBComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	FName HealthStatus = "HStatus";
	
	virtual void OnDeath() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;

	FStopEnteringCover StopEnteringCoverDelegate;
	FStopExitingCover StopExitingCoverDelegate;
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

	UFUNCTION(BlueprintCallable)
	bool IsInCover(); 
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float HealthVisibilityDistance  = 2000.0f;
	
	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float CurrentHealth, float HealthDelta) override;

	virtual TEnumAsByte<ECoverSide> CheckSideByNormal(FVector Forward, FVector Normal);
	virtual TEnumAsByte<ECoverPart> GetCoverPart(int8 PartPos);

	float SideMoveAmount;

private:
	void UpdateHealthWidgetVisibility();
	void UpdateHStateBlackboardKey(uint8 EnumKey);

	//UPROPERTY()
	FCoverData CoverData;
	void CoverCrouch();
};
