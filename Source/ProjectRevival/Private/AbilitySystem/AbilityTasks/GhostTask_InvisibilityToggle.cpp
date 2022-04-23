// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/GhostTask_InvisibilityToggle.h"
#include "PlayerCharacter.h"
#include "AbilitySystem/PRAbilityTypes.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

void UGhostTask_InvisibilityToggle::Activate()
{
	Super::Activate();

	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		Timeline.AddInterpFloat(CurveFloat,TimeLineProgress);
		Timeline.SetLooping(false);
		
		TickActor = GetWorld()->SpawnActorDeferred<ATickActor>(ATickActor::StaticClass(),GetOwnerActor()->GetTransform(), GetOwnerActor());
		TickActor->OnTick.AddUObject(this, &UGhostTask_InvisibilityToggle::TickTimeline);
		UGameplayStatics::FinishSpawningActor(TickActor,GetOwnerActor()->GetTransform());
		//TickActor->SetTimeline(Timeline);
		auto Character = Cast<APlayerCharacter>(GetOwnerActor());
		if (Character)
		{
			Character->GetPlayerSpringArmComponent()->bDoCollisionTest=0;
			Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		}
		DisappearMeshes();
	}
	
}

UGhostTask_InvisibilityToggle* UGhostTask_InvisibilityToggle::InvisibilityToggle(UGameplayAbility* OwningAbility,
	UCurveFloat* FadeCurve, TArray<UMaterialInstanceDynamic*> MeshesMaterials, EChangeWorld CurrentWorld)
{
	const auto AbilityTask = NewAbilityTask<UGhostTask_InvisibilityToggle>(OwningAbility, FName("GhostTask"));
	AbilityTask->CurveFloat = FadeCurve;
	AbilityTask->MeshesMaterials = MeshesMaterials;
	AbilityTask->bTickingTask = 1;
	AbilityTask->BlendIndex = (CurrentWorld==OrdinaryWorld) ? 0 : 2 ;
	return AbilityTask;
}

void UGhostTask_InvisibilityToggle::TickTimeline(float Delta)
{
	if (Timeline.IsPlaying()) Timeline.TickTimeline(Delta);
}

void UGhostTask_InvisibilityToggle::DisappearMeshes()
{
	UE_LOG(LogPRAbilitySystemBase, Display, TEXT("Disappearing meshes has started"))
	
	Timeline.SetTimelineFinishedFunc(OnDisappearFinished);
	Timeline.PlayFromStart();
	//UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Timeline playing status: %s"),Timeline.IsPlaying() ? TEXT("True") : TEXT("False"));
}

void UGhostTask_InvisibilityToggle::AppearMeshes()
{
	UE_LOG(LogPRAbilitySystemBase, Display, TEXT("Appearing meshes has started"))
	auto Character = Cast<APlayerCharacter>(GetOwnerActor());
	if (Character)
	{
		Character->GetPlayerSpringArmComponent()->bDoCollisionTest=1;
		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	}
	Timeline.SetTimelineFinishedFunc(OnAppearFinished);
	Timeline.ReverseFromEnd();
}

/*void UGhostTask_InvisibilityToggle::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Ticking"));
	if (Timeline.IsPlaying())
	{
		Timeline.TickTimeline(DeltaTime);	
	}
}*/


void UGhostTask_InvisibilityToggle::OnDestroy(bool bAbilityEnded)
{
	TickActor->Destroy();
	Super::OnDestroy(bAbilityEnded);
}

void UGhostTask_InvisibilityToggle::TimelineProgress(float Value)
{
	
	for (const auto Material : MeshesMaterials)
	{
		if (Material)
		{
			Material->SetScalarParameterValueByInfo(FMaterialParameterInfo("Amount",
				EMaterialParameterAssociation::BlendParameter,BlendIndex), Value);
			Material->SetScalarParameterValueByInfo(FMaterialParameterInfo("Amount",
				EMaterialParameterAssociation::BlendParameter,0), Value);
		}
			
	}
}
