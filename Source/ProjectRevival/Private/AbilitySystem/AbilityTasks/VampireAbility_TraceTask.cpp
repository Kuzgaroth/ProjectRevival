// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/VampireAbility_TraceTask.h"
#include "AbilitySystem/PRAbilityTypes.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Player/BaseCharacter.h"
#include "BaseWeapon.h"
#include "Kismet/KismetSystemLibrary.h"

UVampireAbility_TraceTask* UVampireAbility_TraceTask::TaskInit(UGameplayAbility* OwningAbility, float Distance)
{
	const auto AbilityTask = NewAbilityTask<UVampireAbility_TraceTask>(OwningAbility, FName("TraceTask"));
	AbilityTask->VampireAbilityDistance = Distance;
	return AbilityTask;
}

void UVampireAbility_TraceTask::Activate()
{
	Super::Activate();
	UE_LOG(LogPRAbilitySystemBase, Error, TEXT("UVampireAbility_TraceTask::Activate"));
	TraceAnalysisStarted();
}

void UVampireAbility_TraceTask::TraceAnalysisStarted()
{
	const APlayerCharacter* Character = Cast<APlayerCharacter>(GetAvatarActor());
	APlayerController* Controller = Cast<APlayerController>(Character->GetController());
    if (!Controller || !GetWorld())
    {
    	UE_LOG(LogPRAbilitySystemBase, Error, TEXT("TraceAnalysisTask failed"));
    	return;
    }
		
	FVector ViewLocation; FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	const FVector TraceStart = ViewLocation;
	const FVector TraceEnd = TraceStart + ViewRotation.Vector() * VampireAbilityDistance;
	Status = false;
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Character);
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore; ActorsToIgnore.Add(GetAvatarActor());
	if(UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility),
	   		false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true))
	{
		if(HitResult.GetActor())
		{
			DamagedCharacter = Cast<ABaseCharacter>(HitResult.GetActor());
			UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Hit actor: %s"), *HitResult.GetActor()->GetName());
			if (DamagedCharacter != nullptr)
			{
				Status = true;
			}
		}
	}
}

void UVampireAbility_TraceTask::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);
}