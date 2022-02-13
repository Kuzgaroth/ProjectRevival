// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityTasks/ChangeWorldTask_SpawnSphere.h"

#include "DrawDebugHelpers.h"

void UChangeWorldTask_SpawnSphere::Activate(AActor& OwnerActor)
{
	Super::Activate();
	const FVector TraceStart=OwnerActor.GetActorLocation();
	const FVector TraceDirection=FVector::DownVector;
	const FVector TraceEnd=TraceStart+TraceDirection*SpawnSphereDistance;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(&OwnerActor);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,ECollisionChannel::ECC_Visibility,CollisionParams);
	if(HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(),TraceStart,HitResult.ImpactPoint,FColor::Blue,false,3.0f,0,3.0f);
		DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,10.0f,24,FColor::Red,false,5.0f);
	}
	
}
UChangeWorldTask_SpawnSphere* UChangeWorldTask_SpawnSphere::ChangeWorldInit(UGameplayAbility* OwningAbility,TSubclassOf<AActor> ChangeWorldSphere,float TraceDistance )
{
	const auto AbilityTask = NewAbilityTask<UChangeWorldTask_SpawnSphere>(OwningAbility, FName("ChangeWorldTask"));
	AbilityTask->ChangeWorldSphere=ChangeWorldSphere;
	AbilityTask->SpawnSphereDistance=TraceDistance;
	return AbilityTask;
}
