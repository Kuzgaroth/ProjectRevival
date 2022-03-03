#include "AbilitySystem/Abilities/VampireAbility.h"
#include "Components/WeaponComponent.h"
#include "AbilitySystem/AbilityTasks/VampireAbility_TraceTask.h"

UVampireAbility::UVampireAbility()
{
	AbilityAction = EGASInputActions::Vampire;
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
		APlayerCharacter* Character = Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get());
		APlayerController* Controller = Cast<APlayerController>(Character->GetController());
		UWeaponComponent* Weapon = Cast<UWeaponComponent>(Character->GetWeaponComponent());
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		
		if(Character->GetCharacterMovement()->IsFlying()||Character->GetCharacterMovement()->IsFalling() &&
			VampireReverseMontage == nullptr && VampireMontage == nullptr && AnimInstance == nullptr)
		{
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("No montage founded"));
			K2_EndAbility();
  		}
		else
		{
			if(Weapon->IsShooting())
			{
				Weapon->StopFire();
			}
						
			TraceTask = UVampireAbility_TraceTask::TaskInit(this, VampireAbilityDistance, VampireAbilityDamage);
			TraceTask->Activate();
			
			if(TraceTask->Status)
			{
				MontageDuration = VampireMontage->GetPlayLength();
				AddNewBeam();
				MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None,
				   VampireMontage, true, NAME_None, true);
				MontageTask->ReadyForActivation();
				UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("trace match"));
			}
			else
			{
				MontageDuration = VampireReverseMontage->GetPlayLength();
				MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None,
				   VampireReverseMontage, true, NAME_None, true);
				MontageTask->ReadyForActivation();
				UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("No trace match"));
			}
			DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, MontageDuration);
			DelayTask->OnFinish.AddDynamic(this, &UVampireAbility::OnTraceAnalysisEnd);
			DelayTask->ReadyForActivation();
		}
	}
}

void UVampireAbility::AddNewBeam()
{
	//const FVector TraceStart = WeaponMesh->GetSocketLocation("TraceStart");
	//const FVector TraceEnd = WeaponMesh->GetSocketLocation("TraceEnd");
	if (BeamComp)
	{
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Beam added"));
		//BeamComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamFX, TraceStart, FRotator::ZeroRotator, true);
		//BeamArray.Add(BeamComp);
		//BeamComp->SetBeamSourcePoint(0, TraceStart, 0);
		//BeamComp->SetBeamTargetPoint(0, TraceEnd, 0);
	}
}

void UVampireAbility::OnTraceAnalysisEnd()
{
	//TraceTask->FlipFinished();
	//TraceTask->EndTask();
	K2_EndAbility();
}

void UVampireAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}