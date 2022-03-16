// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/GrenadeTask_ThrowGrenade.h"
#include "BaseCharacter.h"
#include "AbilitySystem/AbilityActors/Grenades/BaseGrenade.h"
#include "Kismet/GameplayStatics.h"

void UGrenadeTask_ThrowGrenade::Activate()
{
	Super::Activate();
	FTransform TempTransform;
	TempTransform.SetLocation(Cast<ABaseCharacter>(AbilityHead->GetAvatarActorFromActorInfo())->GetMesh()->GetSocketLocation(GrenadeSocketNamePassed));
	TempTransform.SetRotation(AbilityHead->GetAvatarActorFromActorInfo()->GetActorRotation().Quaternion());
	ABaseGrenade* SpawnedGrenade = Cast<ABaseGrenade>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), GrenadeToUsePassed, TempTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
		AbilityHead->GetAvatarActorFromActorInfo()->GetOwner()));
	if (SpawnedGrenade)
	{
		SpawnedGrenade->SetInitialSpeed(ThrowGrenadeForcePassed);
		SpawnedGrenade->FinishSpawning(TempTransform);
	}
}

UGrenadeTask_ThrowGrenade* UGrenadeTask_ThrowGrenade::ThrowGrenade(UGameplayAbility* OwningAbility, TSubclassOf<ABaseGrenade> GrenadeToUse, float ThrowGrenadeForce, FName GrenadeSocketName)
{
	UGrenadeTask_ThrowGrenade* AbilityTask = NewAbilityTask<UGrenadeTask_ThrowGrenade>(OwningAbility, FName("GrenadeTask"));
	AbilityTask->AbilityHead = OwningAbility;
	AbilityTask->ThrowGrenadeForcePassed = ThrowGrenadeForce;
	AbilityTask->GrenadeSocketNamePassed = GrenadeSocketName;
	AbilityTask->GrenadeToUsePassed = GrenadeToUse;
	return AbilityTask;
}

void UGrenadeTask_ThrowGrenade::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}