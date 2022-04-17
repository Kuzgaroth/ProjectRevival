// Project Revival. All Rights Reserved

#include "AbilitySystem/AbilityTasks/VisorSwitcherTask_TriggerVisor.h"
#include "AbilitySystem/Abilities/Miscellaneuos/VisorItemAttached.h"
#include "Kismet/GameplayStatics.h"

void UVisorSwitcherTask_TriggerVisor::Activate()
{
	Super::Activate();
	FTransform TempTransform;
	TempTransform.SetLocation(AbilityHead->GetAvatarActorFromActorInfo()->GetTransform().GetLocation());
	TempTransform.SetRotation(AbilityHead->GetAvatarActorFromActorInfo()->GetTransform().GetRotation());
	AVisorItemAttached* SpawnedVisorItem = Cast<AVisorItemAttached>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AVisorItemAttached::StaticClass(), TempTransform,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,AbilityHead->GetAvatarActorFromActorInfo()->GetInstigator()));
	if (SpawnedVisorItem)
	{
		SpawnedVisorItem->AttachToActor(AbilityHead->GetAvatarActorFromActorInfo(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SpawnedVisorItem->TurnOffDelay = Duration;
		SpawnedVisorItem->VisorRadius = Radius;
		SpawnedVisorItem->PlayedEffect = EffectToShow;
		// SpawnedVisorItem->SetReplaceMaterial(MaterialToSet);
		SpawnedVisorItem->SetObjectTypesToVisor(ObjectTypesForVisoring);
		UGameplayStatics::FinishSpawningActor(SpawnedVisorItem, TempTransform);
		SpawnedVisorRef = SpawnedVisorItem;
	}
}

UVisorSwitcherTask_TriggerVisor* UVisorSwitcherTask_TriggerVisor::SpawnVisor(UGameplayAbility* OwningAbility, float VisorRadius, float VisorDuration,
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToVisor, UParticleSystem* ParticleEffect, class UMaterial* ReplaceMaterial)
{
	const auto AbilityTask = NewAbilityTask<UVisorSwitcherTask_TriggerVisor>(OwningAbility, FName("VisorTask"));
	AbilityTask->MaterialToSet = ReplaceMaterial;
	AbilityTask->AbilityHead = OwningAbility;
	AbilityTask->Radius = VisorRadius;
	AbilityTask->Duration = VisorDuration;
	AbilityTask->EffectToShow = ParticleEffect;
	AbilityTask->ObjectTypesForVisoring = ObjectTypesToVisor;
	return AbilityTask;
}

void UVisorSwitcherTask_TriggerVisor::OnDestroy(bool bAbilityEnded)
{
	if (SpawnedVisorRef) Cast<AVisorItemAttached>(SpawnedVisorRef)->StartDestructionCountdown();
	Super::OnDestroy(bAbilityEnded);
}