// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/GrenadeTask_ThrowGrenade.h"
#include "BaseCharacter.h"
#include "AbilitySystem/AbilityActors/Grenades/BaseGrenade.h"
#include "Kismet/GameplayStatics.h"

void UGrenadeTask_ThrowGrenade::Activate()
{
	Super::Activate();
	// UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Speed in task at start: %f"), ThrowGrenadeForcePassed);
	FTransform TempTransform;
	TempTransform.SetLocation(Cast<ABaseCharacter>(AbilityHead->GetAvatarActorFromActorInfo())->GetMesh()->GetSocketLocation(GrenadeSocketNamePassed));
	TempTransform.SetRotation(AbilityHead->GetAvatarActorFromActorInfo()->GetActorRotation().Quaternion());
	ABaseGrenade* SpawnedGrenade = Cast<ABaseGrenade>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), GrenadeToUsePassed, TempTransform,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,AbilityHead->GetAvatarActorFromActorInfo()->GetInstigator()));
	if (SpawnedGrenade)
	{
		// UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Owner of task is %s"), *AbilityHead->GetAvatarActorFromActorInfo()->GetInstigator()->GetName());
		// UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Current grenade is %s"), *SpawnedGrenade->GetName());
		// UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Speed in task before Set: %f"), ThrowGrenadeForcePassed);
		SpawnedGrenade->SetInitialSpeed(ThrowGrenadeForcePassed);
		UGameplayStatics::FinishSpawningActor(SpawnedGrenade, TempTransform);
		UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Spawn of grenade is finished"));
	}
}

UGrenadeTask_ThrowGrenade* UGrenadeTask_ThrowGrenade::ThrowGrenade(UGameplayAbility* OwningAbility, TSubclassOf<ABaseGrenade> GrenadeToUse, float ThrowGrenadeForce, FName GrenadeSocketName)
{
	UGrenadeTask_ThrowGrenade* AbilityTask = NewAbilityTask<UGrenadeTask_ThrowGrenade>(OwningAbility, FName("GrenadeTask"));
	AbilityTask->AbilityHead = OwningAbility;
	// UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Speed passed in TaskActivate from ability: %f"), ThrowGrenadeForce);
	AbilityTask->ThrowGrenadeForcePassed = ThrowGrenadeForce;
	// UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Speed set to task: %f"), AbilityTask->ThrowGrenadeForcePassed);
	AbilityTask->GrenadeSocketNamePassed = GrenadeSocketName;
	AbilityTask->GrenadeToUsePassed = GrenadeToUse;
	return AbilityTask;
}

void UGrenadeTask_ThrowGrenade::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}