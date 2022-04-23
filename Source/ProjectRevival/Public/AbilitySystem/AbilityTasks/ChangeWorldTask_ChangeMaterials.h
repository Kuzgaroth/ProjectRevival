// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ChangeWorldTask_ChangeMaterials.generated.h"


UCLASS()
class PROJECTREVIVAL_API UChangeWorldTask_ChangeMaterials : public UAbilityTask
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UChangeWorldTask_ChangeMaterials* ChangeMaterials(UGameplayAbility* OwningAbility, UCurveFloat* ChangeCurve, TArray<UMaterialInstanceDynamic*> MeshesMaterials, EChangeWorld CurrentWorld);

	
	
	UFUNCTION()
	void TickTimeline(float Delta);
protected:
	void DisappearMeshes();
	UPROPERTY()
	int32 BlendIndex=1;
private:
	virtual void OnDestroy(bool AbilityEnded) override;
	UFUNCTION()
	void TimelineProgress(float Value);
	
	UPROPERTY()
	ATickActor* TickActor;
	
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* CurveFloat;
	
	FTimeline Timeline;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MeshesMaterials;
};
