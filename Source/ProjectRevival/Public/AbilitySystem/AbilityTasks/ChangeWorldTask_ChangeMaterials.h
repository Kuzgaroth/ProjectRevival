// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "FuncActors/TickActor.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "ChangeWorldTask_ChangeMaterials.generated.h"


UCLASS()
class PROJECTREVIVAL_API UChangeWorldTask_ChangeMaterials : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UChangeWorldTask_ChangeMaterials* ChangeMaterials(UGameplayAbility* OwningAbility, UCurveFloat* ChangeCurve, TArray<UMaterialInstanceDynamic*> MeshesMaterials, EChangeWorld CurrentWorld);
	virtual void Activate() override;
	UFUNCTION()
	void TickTimeline(float Delta);
protected:
	UPROPERTY()
	int32 BlendIndex=1;
	FOnTimelineEvent TransitionEndedEvent;
private:
	virtual void OnDestroy(bool AbilityEnded) override;
	UFUNCTION()
	void TimelineProgress(float Value);
	UFUNCTION()
	void OnTransitionEnded();
	UPROPERTY()
	ATickActor* TickActor;
	
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* CurveFloat;
	EChangeWorld CurrentWorld;
	FTimeline Timeline;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MeshesMaterials;
};
