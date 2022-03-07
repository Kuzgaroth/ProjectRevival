// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/VampireAbility_TraceTask.h"
#include "AbilitySystem/PRAbilityTypes.h"
#include "PlayerCharacter.h"
#include "Player/BaseCharacter.h"
#include "WeaponComponent.h"
#include "BaseWeapon.h"
#include "HealthComponent.h"


UVampireAbility_TraceTask* UVampireAbility_TraceTask::TaskInit(UGameplayAbility* OwningAbility, float Distance, float Damage)
{
	const auto AbilityTask = NewAbilityTask<UVampireAbility_TraceTask>(OwningAbility, FName("TraceTask"));
	AbilityTask->VampireAbilityDistance = Distance;
	AbilityTask->VampireAbilityDamage = Damage;
	return AbilityTask;
}

void UVampireAbility_TraceTask::Activate()
{
	Super::Activate();
	UE_LOG(LogPRAbilitySystemBase, Error, TEXT("UVampireAbility_TraceTask::Activate"));
	//if (CurveFloat)
	{
		//FOnTimelineFloat TimeLineProgress;
		//TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		//Timeline.AddInterpFloat(CurveFloat,TimeLineProgress);
		//Timeline.SetLooping(false);
		TraceAnalysisStarted();
	}
}

void UVampireAbility_TraceTask::TickTimeline(float Delta)
{
	if (Timeline.IsPlaying()) Timeline.TickTimeline(Delta);
}

void UVampireAbility_TraceTask::TraceAnalysisStarted()
{
	FVector ViewLocation;
  	FRotator ViewRotation;
	
	APlayerCharacter* Character = Cast<APlayerCharacter>(GetAvatarActor());
	//UWeaponComponent* WeaponComponent = Cast<UWeaponComponent>(Character->GetWeaponComponent());
	APlayerController* Controller = Cast<APlayerController>(Character->GetController());
    if (!Controller)
    {
    	UE_LOG(LogPRAbilitySystemBase, Error, TEXT("!Controller"));
    	return;
    }
	if (!GetWorld())
	{
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("!GetWorld"));
		return;
	}
	
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	FVector TraceStart = ViewLocation;
	FVector TraceEnd = TraceStart + ViewRotation.Vector() * VampireAbilityDistance;
	 	
 	FHitResult HitResult;	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Character);
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams))
	{
		FVector TraceFXEnd = TraceEnd;
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("HitResult.bBlockingHit"));
		Status = true;
		TraceFXEnd = HitResult.ImpactPoint;
		//DrawDebugLine(GetWorld(),GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
		MakeDamage(HitResult, Character, Controller);
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("TraceAnalysisStarted done"));
		//Weapon->WeaponFXComponent->PlayImpactFX(HitResult);
	}
	
	if (HitResult.GetActor())
	{
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Hit actor: %s"), *HitResult.GetActor()->GetName());
	}
	TraceAnalysisFinished();
}

void UVampireAbility_TraceTask::TraceAnalysisFinished()
{
	UE_LOG(LogPRAbilitySystemBase, Error, TEXT("TraceAnalysisFinished"));
	//Timeline.SetTimelineFinishedFunc(OnAnalysisFinished);
	//GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UVampireAbility_TraceTask::MakeDamage(FHitResult& HitResult, APlayerCharacter* Character, APlayerController* Controller)
{	
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor)
	{
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("!DamagedActor"));
		return;
	}

	DamagedActor->TakeDamage(VampireAbilityDamage,FDamageEvent{},Controller,Character);
	if(Character->GetHealthComponent())
	{
		Character->GetHealthComponent()->TryToAddHealth(VampireAbilityDamage);
	}
	else
	{
		UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("!GetHealthComponent"));
	}
}

void UVampireAbility_TraceTask::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}
