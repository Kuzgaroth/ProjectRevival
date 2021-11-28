// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/GhostAbility.h"
#include "AbilitySystem/Abilities/Miscellaneuos/IDynMaterialsFromMesh.h"

UGhostAbility::UGhostAbility()
{
}

void UGhostAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	//далее идет место с твоей логикой, туда ты вставляешь свой код
	//--------------------------------------------------------------------
	const auto Owner = ActorInfo->OwnerActor.Get();
	if (!Owner->Implements<UIDynMaterialsFromMesh>())
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Owner actor MUST implement IIDynMaterialsFromMesh interface!!!"))
		K2_EndAbility();
	}
	GhostTask = UGhostTask_InvisibilityToggle::InvisibilityToggle(this, VisualCurve, Cast<IIDynMaterialsFromMesh>(Owner)->GetDynMaterials());
	GhostTask->OnDisappearFinished.BindUFunction(this, "OnDisappearEnded");
	GhostTask->OnAppearFinished.BindUFunction(this, "OnAppearEnded");
	DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, Duration);
	DelayTask->OnFinish.AddDynamic(this, &UGhostAbility::BeginAppear);
	
	GhostTask->Activate();
	//--------------------------------------------------------------------
	
}

void UGhostAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                               bool bWasCancelled)
{
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGhostAbility::OnAppearEnded()
{
	GhostTask->OnAppearFinished.Unbind();
	GhostTask->EndTask();
	K2_EndAbility();
}

void UGhostAbility::OnDisappearEnded()
{
	GhostTask->OnDisappearFinished.Unbind();
	DelayTask->Activate();	
}

void UGhostAbility::BeginAppear()
{
	DelayTask->OnFinish.RemoveDynamic(this, &UGhostAbility::BeginAppear);
	GhostTask->AppearMeshes();
}
