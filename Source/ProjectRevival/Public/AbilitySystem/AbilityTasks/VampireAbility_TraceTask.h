// Project Revival. All Rights Reserved

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "PlayerCharacter.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "VampireAbility_TraceTask.generated.h"

UCLASS()
class PROJECTREVIVAL_API UVampireAbility_TraceTask : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;

	UPROPERTY(EditDefaultsOnly, Category="Flip constants")
	float VampireAbilityDistance;
	
	UPROPERTY(EditDefaultsOnly, Category="Flip constants")
	float VampireAbilityDamage;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UVampireAbility_TraceTask* TaskInit(UGameplayAbility* OwningAbility, float Distance, float Damage);

	UPROPERTY(BlueprintReadOnly)
	bool Status = false;
	
private:
	void TraceAnalysisStarted();
	void TraceAnalysisFinished();
	FOnTimelineEvent OnAnalysisStarted;
	FOnTimelineEvent OnAnalysisFinished;
		
	UFUNCTION()
	void TickTimeline(float Delta);
	FTimeline Timeline;

	void MakeDamage(FHitResult& HitResult, APlayerCharacter* Character, APlayerController* Controller);
	
	virtual void OnDestroy(bool bAbilityEnded) override;
};
