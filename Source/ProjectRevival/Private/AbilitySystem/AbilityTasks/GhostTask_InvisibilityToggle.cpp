// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/GhostTask_InvisibilityToggle.h"
#include "AbilitySystem/PRAbilityTypes.h"

void UGhostTask_InvisibilityToggle::Activate()
{
	Super::Activate();

	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		Timeline.AddInterpFloat(CurveFloat,TimeLineProgress);
		Timeline.SetLooping(false);

		DisappearMeshes();
	}
	
}

UGhostTask_InvisibilityToggle* UGhostTask_InvisibilityToggle::InvisibilityToggle(UGameplayAbility* OwningAbility,
	UCurveFloat* FadeCurve, TArray<UMaterialInstanceDynamic*>& MeshesMaterials)
{
	const auto AbilityTask = NewAbilityTask<UGhostTask_InvisibilityToggle>(OwningAbility, FName("GhostTask"));
	AbilityTask->CurveFloat = FadeCurve;
	AbilityTask->MeshesMaterials = MeshesMaterials;
	return AbilityTask;
}

void UGhostTask_InvisibilityToggle::DisappearMeshes()
{
	UE_LOG(LogPRAbilitySystemBase, Display, TEXT("Disappearing meshes has started"))
	Timeline.SetTimelineFinishedFunc(OnDisappearFinished);
	Timeline.PlayFromStart();
}

void UGhostTask_InvisibilityToggle::AppearMeshes()
{
	UE_LOG(LogPRAbilitySystemBase, Display, TEXT("Appearing meshes has started"))
	Timeline.SetTimelineFinishedFunc(OnAppearFinished);
	Timeline.ReverseFromEnd();
}

void UGhostTask_InvisibilityToggle::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (Timeline.IsPlaying())
	{
		Timeline.TickTimeline(DeltaTime);	
	}
}


void UGhostTask_InvisibilityToggle::OnDestroy(bool bAbilityEnded)
{
	
	Super::OnDestroy(bAbilityEnded);
}

void UGhostTask_InvisibilityToggle::TimelineProgress(float Value)
{
	for (const auto Material : MeshesMaterials)
	{
		Material->SetScalarParameterValue("Amount",Value);
	}
}
