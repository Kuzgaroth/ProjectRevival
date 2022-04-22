// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/ChangeWorldTask_SpawnSphere.h"

#include "DrawDebugHelpers.h"
#include "AbilitySystem/AbilityActors/ChangeWorldSphereActor.h"

void UChangeWorldTask_SpawnSphere::Activate()
{
	Super::Activate();

	
}
AChangeWorldSphereActor* UChangeWorldTask_SpawnSphere::StartTask(AActor& OwnerActor)
{
	Activate();
	const FVector TraceStart=OwnerActor.GetActorLocation();
	const FVector TraceDirection=FVector::DownVector;
	const FVector TraceEnd=TraceStart+TraceDirection*SpawnSphereDistance;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(&OwnerActor);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,ECollisionChannel::ECC_Visibility,CollisionParams);
	DrawDebugLine(GetWorld(),TraceStart,HitResult.ImpactPoint,FColor::Blue,false,3.0f,0,3.0f);
	DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,10.0f,24,FColor::Red,false,5.0f);
	if(HitResult.bBlockingHit)
	{
		
		FActorSpawnParameters SpawnParams;
		if(ChangeWorldSphere)
		{
			auto act=GetWorld()->SpawnActor<AChangeWorldSphereActor>(ChangeWorldSphere,HitResult.ImpactPoint,FRotator::ZeroRotator,SpawnParams);
			act->SetOwner(&OwnerActor);
			return act;
		}
		return nullptr;
		
	}
	return nullptr;
}
UChangeWorldTask_SpawnSphere* UChangeWorldTask_SpawnSphere::ChangeWorldInit(UGameplayAbility* OwningAbility,TSubclassOf<AActor> ChangeWorldSphere,float TraceDistance )
{
	const auto AbilityTask = NewAbilityTask<UChangeWorldTask_SpawnSphere>(OwningAbility, FName("ChangeWorldTask"));
	AbilityTask->ChangeWorldSphere=ChangeWorldSphere;
	AbilityTask->SpawnSphereDistance=TraceDistance;
	return AbilityTask;
}
