// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/ChangeWorldTask_ChangeMaterials.h"

#include "Kismet/GameplayStatics.h"

UChangeWorldTask_ChangeMaterials* UChangeWorldTask_ChangeMaterials::ChangeMaterials(UGameplayAbility* OwningAbility,
                                                                                    UCurveFloat* ChangeCurve, TArray<UMaterialInstanceDynamic*> MeshesMaterials, EChangeWorld CurrentWorld)
{
	const auto AbilityTask = NewAbilityTask<UChangeWorldTask_ChangeMaterials>(OwningAbility, FName("ChangeWorldMaterials"));
	AbilityTask->CurveFloat = ChangeCurve;
	AbilityTask->MeshesMaterials = MeshesMaterials;
	AbilityTask->bTickingTask = 1;
	AbilityTask->CurrentWorld = CurrentWorld;
	return AbilityTask;
}

void UChangeWorldTask_ChangeMaterials::Activate()
{
	Super::Activate();
	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		Timeline.AddInterpFloat(CurveFloat,TimeLineProgress);
		Timeline.SetLooping(false);
		
		TickActor = GetWorld()->SpawnActorDeferred<ATickActor>(ATickActor::StaticClass(),GetOwnerActor()->GetTransform(), GetOwnerActor());
		TickActor->OnTick.AddUObject(this, &UChangeWorldTask_ChangeMaterials::TickTimeline);
		UGameplayStatics::FinishSpawningActor(TickActor,GetOwnerActor()->GetTransform());

		TransitionEndedEvent.BindDynamic(this, &UChangeWorldTask_ChangeMaterials::OnTransitionEnded);
		
		Timeline.SetTimelineFinishedFunc(TransitionEndedEvent);
		Timeline.PlayFromStart();
	}
}

void UChangeWorldTask_ChangeMaterials::TickTimeline(float Delta)
{
	if (Timeline.IsPlaying()) Timeline.TickTimeline(Delta);
}

void UChangeWorldTask_ChangeMaterials::OnDestroy(bool AbilityEnded)
{
	TickActor->Destroy();
	Super::OnDestroy(AbilityEnded);
}

void UChangeWorldTask_ChangeMaterials::TimelineProgress(float Value)
{
	const float Point = (CurrentWorld == OrdinaryWorld) ? 1-Value : Value;
	for (const auto Material : MeshesMaterials)
	{
		if (Material)
		{
			Material->SetScalarParameterValueByInfo(FMaterialParameterInfo("Amount",
				EMaterialParameterAssociation::BlendParameter,BlendIndex), Point);
			
		}
	}
}

void UChangeWorldTask_ChangeMaterials::OnTransitionEnded()
{
	TransitionEndedEvent.Clear();
	EndTask();
}