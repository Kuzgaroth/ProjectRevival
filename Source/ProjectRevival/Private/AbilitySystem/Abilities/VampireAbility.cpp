#include "AbilitySystem/Abilities/VampireAbility.h"

#include "HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "AbilitySystem/AbilityTasks/VampireAbility_TraceTask.h"
#include "Kismet/GameplayStatics.h"

UVampireAbility::UVampireAbility()
{
	AbilityAction = EGASInputActions::Vampire;
	BeamComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam Component"));
	BeamComp->bAutoActivate = false;
}

void UVampireAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("UVampireAbility::CommitExecute"));
	const auto Owner = ActorInfo->OwnerActor.Get();
	if (!Owner)
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Unable to get Owner Actor"));
		K2_EndAbility();
	}
	else
	{
		const APlayerCharacter* Character = Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get());
		UWeaponComponent* Weapon = Cast<UWeaponComponent>(Character->GetWeaponComponent());
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		
		if(Character->GetCharacterMovement()->IsFlying()||Character->GetCharacterMovement()->IsFalling() ||
			VampireReverseMontage == nullptr || VampireMontage == nullptr || AnimInstance == nullptr ||
			!Weapon->CanFire())
		{
			UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Impossible to start VampireAbility"));
			K2_EndAbility();
  		}
		else
		{
			Weapon->SetWeaponBlocked(true);
			if(Weapon->IsShooting())
			{
				Weapon->StopFire();
			}
						
			TraceTask = UVampireAbility_TraceTask::TaskInit(this, VampireAbilityDistance);
			TraceTask->Activate();
			
			MontageDuration = VampireMontage->GetPlayLength();
			MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None,
				VampireMontage, MontageRate, NAME_None, true);
			MontageTask->ReadyForActivation();
			
			DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, MontageDuration / MontageRate);
			DelayTask->OnFinish.AddDynamic(this, &UVampireAbility::OnTraceAnalysisEnd);
			DelayTask->ReadyForActivation();
		}
	}
}

void UVampireAbility::OnTraceAnalysisEnd()
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(CurrentActorInfo->OwnerActor.Get());
 	APlayerController* Controller = Cast<APlayerController>(Character->GetController());
 	UWeaponComponent* Weapon = Cast<UWeaponComponent>(Character->GetWeaponComponent());
	
	if(TraceTask->Status)
	{
		PlayVFX();
		MakeDamage(TraceTask->DamagedCharacter, Character, Controller);
		Weapon->SetWeaponBlocked(false);
		TraceTask->EndTask();
		K2_EndAbility();
	}
	else
	{
		MontageDuration = VampireReverseMontage->GetPlayLength();
		MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None,
			VampireReverseMontage, MontageRate, NAME_None, true);
		MontageTask->ReadyForActivation();
		
		DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, MontageDuration / MontageRate);
		DelayTask->OnFinish.AddDynamic(this, &UVampireAbility::OnReverseMontageEnd);
		DelayTask->ReadyForActivation();
		
		TraceTask->EndTask();
	}
	
}

void UVampireAbility::OnReverseMontageEnd()
{
	const APlayerCharacter* Character = Cast<APlayerCharacter>(CurrentActorInfo->OwnerActor.Get());
	UWeaponComponent* Weapon = Cast<UWeaponComponent>(Character->GetWeaponComponent());
	Weapon->SetWeaponBlocked(false);
	K2_EndAbility();
}

void UVampireAbility::PlayVFX()
{
	if (!GetWorld())
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("PlayVFX failed"));
		return;
	}
	const FVector TraceEnd = TraceTask->DamagedCharacter->GetActorLocation();
	BeamComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamFX, TraceEnd, FRotator::ZeroRotator, true);
	BeamArray.Add(BeamComp);
}

void UVampireAbility::MakeDamage(ABaseCharacter* DamagedActor, APlayerCharacter* Character, APlayerController* Controller) const
{	
	if (!DamagedActor)
	{
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Impossible to get damaged actor"));
		return;
	}

	DamagedActor->TakeDamage(VampireAbilityDamage,FDamageEvent{},Controller,Character);
	if(Character->GetHealthComponent())
	{
		Character->GetHealthComponent()->TryToAddHealth(VampireAbilityDamage);
	}
	else
	{
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Impossible to get player's HealthComponent"));
	}
}

void UVampireAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}