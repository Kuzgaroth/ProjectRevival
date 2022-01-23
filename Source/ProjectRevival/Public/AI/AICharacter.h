// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AI/Soldier/SoldierAIController.h"
#include "EnvironmentQuery/EQSTestingPawn.h"
#include "Player/BaseCharacter.h"
#include "AICharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPRAICharacter, Log, All)

class UBehaviorTree;
class UWidgetComponent;

UENUM(BlueprintType)
enum class HealthState: uint8
{
	NEAR_DEATH,
	BADLY_INJURED,
	SLIGHTLY_INJURED,
	FULL_HEALTH
};

UCLASS()
class PROJECTREVIVAL_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AAICharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	HealthState HState;

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
	void StartFiring(const FVector& PlayerPos);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float HealthVisibilityDistance  = 2000.0f;
	
	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float CurrentHealth, float HealthDelta) override;
private:
	void UpdateHealthWidgetVisibility();
	void UpdateHStateBlackboardKey(uint8 EnumKey);
};
