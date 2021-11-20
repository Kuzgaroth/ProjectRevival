// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Components/TimelineComponent.h"
#include "GhostTask_InvisibilityToggle.generated.h"



UCLASS()
class PROJECTREVIVAL_API UGhostTask_InvisibilityToggle : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UGhostTask_InvisibilityToggle* InvisibilityToggle(UGameplayAbility* OwningAbility, UCurveFloat* FadeCurve, TArray<UMaterialInstanceDynamic*>& MeshesMaterials);

	
	void AppearMeshes();
	FOnTimelineEvent OnAppearFinished;
	FOnTimelineEvent OnDisappearFinished;
protected:
	virtual void TickTask(float DeltaTime) override;
	void DisappearMeshes();
private:
	virtual void OnDestroy(bool AbilityEnded) override;
	UFUNCTION()
	void TimelineProgress(float Value);


	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* CurveFloat;
	
	FTimeline Timeline;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MeshesMaterials;
};
