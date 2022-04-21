// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/GhostAbility.h"
#include "AbilitySystem/Abilities/Miscellaneuos/IDynMaterialsFromMesh.h"
#include "Kismet/GameplayStatics.h"

UGhostAbility::UGhostAbility()
{
	AbilityAction = EGASInputActions::Ghost; 
}

void UGhostAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	//далее идет место с твоей логикой, туда ты вставляешь свой код.
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

	PlayGhostSoundEffect();
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
	ChangeBlendMode(false);
	GhostTask->EndTask();
	PlayGhostSoundEffect();
	K2_EndAbility();
}

void UGhostAbility::OnDisappearEnded()
{
	GhostTask->OnDisappearFinished.Unbind();
	ChangeBlendMode(true);
	DelayTask->Activate();	
}

void UGhostAbility::PlayGhostSoundEffect()
{
	if (!GhostCue) return;

	UGameplayStatics::SpawnSoundAttached(GhostCue,GetOwningActorFromActorInfo()->GetRootComponent());
}

void UGhostAbility::BeginAppear()
{
	DelayTask->OnFinish.RemoveDynamic(this, &UGhostAbility::BeginAppear);
	GhostTask->AppearMeshes();
}

void UGhostAbility::ChangeBlendMode(bool IsDisappearing)
{
	auto Materials = Cast<IIDynMaterialsFromMesh>((GetCurrentActorInfo()->OwnerActor.Get()))->GetDynMaterials();
	for (const auto Material : Materials)
	{
		Material->BlendMode = IsDisappearing ? TEnumAsByte<EBlendMode>(EBlendMode::BLEND_Translucent) : TEnumAsByte<EBlendMode>(EBlendMode::BLEND_Opaque);
	}
}
