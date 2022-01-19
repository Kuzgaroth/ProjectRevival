// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "EnvironmentQuery/EQSTestingPawn.h"
#include "Player/BaseCharacter.h"
#include "SoldierEnemy.generated.h"

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

UENUM(BlueprintType)
enum class CoverType: uint8
{
	NEAR_DEATH,
	BADLY_INJURED,
	SLIGHTLY_INJURED,
	FULL_HEALTH
};

UCLASS()
class PROJECTREVIVAL_API ASoldierEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	ASoldierEnemy(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	HealthStateSoldier HState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	ABaseAIController* AICon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBlackboardComponent* BBComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	FName HealthStatus = "HStatus";

	UFUNCTION(BlueprintCallable)
	virtual bool IsCovering() const;
	
	virtual void OnDeath() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float HealthVisibilityDistance  = 2000.0f;
	
	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float CurrentHealth, float HealthDelta) override;
private:
	bool bWantsToCover = false;
	bool IsMovingToCover = false;
	void UpdateHealthWidgetVisibility();
	void UpdateHStateBlackboardKey(uint8 EnumKey);
	void StartCovering();
	void StopCovering();
};
