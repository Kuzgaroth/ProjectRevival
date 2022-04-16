// Project Revival. All Rights Reserved

#include "AbilitySystem/AbilityTasks/VisorPlacerTask_SpawnVisor.h"
#include "AbilitySystem/Abilities/Miscellaneuos/VisorItem.h"
#include "Kismet/GameplayStatics.h"

void UVisorPlacerTask_SpawnVisor::Activate()
{
	Super::Activate();
	FTransform TempTransform;
	TempTransform.SetLocation(AbilityHead->GetAvatarActorFromActorInfo()->GetTransform().GetLocation());
	TempTransform.SetRotation(AbilityHead->GetAvatarActorFromActorInfo()->GetTransform().GetRotation());
	AVisorItem* SpawnedVisorItem = GetWorld()->SpawnActorDeferred<AVisorItem>(AVisorItem::StaticClass(), TempTransform,
		Cast<AActor>(AbilityHead->GetAvatarActorFromActorInfo()->GetInstigator()),
		Cast<APawn>(AbilityHead->GetAvatarActorFromActorInfo()->GetInstigator()),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (SpawnedVisorItem)
	{
		SpawnedVisorItem->VisorDuration = Duration;
		SpawnedVisorItem->VisorRadius = Radius;
		SpawnedVisorItem->PlayedEffect = EffectToShow;
		SpawnedVisorItem->SetObjectTypesToVisor(ObjectTypesForVisoring);
		UGameplayStatics::FinishSpawningActor(SpawnedVisorItem, TempTransform);
	}
}

UVisorPlacerTask_SpawnVisor* UVisorPlacerTask_SpawnVisor::SpawnVisor(UGameplayAbility* OwningAbility, float VisorRadius, float VisorDuration,
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToVisor, UParticleSystem* ParticleEffect)
{
	const auto AbilityTask = NewAbilityTask<UVisorPlacerTask_SpawnVisor>(OwningAbility, FName("VisorTask"));
	AbilityTask->AbilityHead = OwningAbility;
	AbilityTask->Radius = VisorRadius;
	AbilityTask->Duration = VisorDuration;
	AbilityTask->EffectToShow = ParticleEffect;
	AbilityTask->ObjectTypesForVisoring = ObjectTypesToVisor;
	return AbilityTask;
}

void UVisorPlacerTask_SpawnVisor::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}