// Project Revival. All Rights Reserved


#include "GameFeature/CoverObject.h"

#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"

// Sets default values
ACoverObject::ACoverObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsTaken = false;

}

// Called when the game starts or when spawned
void ACoverObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoverObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACoverObject::TryToFindCoverPoint(FVector PlayerPos, FVector& CoverPos)
{
	if(CoverStruct.CoverPositions.Num()==0) return false;
	for(auto covpos:CoverStruct.CoverPositions)
	{
		FVector TraceStart=covpos->GetComponentLocation();
		FVector TraceEnd=PlayerPos;
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,ECollisionChannel::ECC_Visibility,CollisionParams);
		if(HitResult.bBlockingHit)
		{
			DrawDebugLine(GetWorld(),TraceStart,HitResult.ImpactPoint,FColor::Blue,false,3.0f,0,3.0f);
			if(HitResult.Actor==this)
			{
				
				CoverPos=TraceStart;
				return true;
			}
		}
	}
	return false;
}

#if WITH_EDITOR
void ACoverObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if(PropertyChangedEvent.Property->GetName()=="CoverStruct")
	{
		CoverStruct.PositionsOfCoverPoints.Empty();
		for(auto covpos:CoverStruct.CoverPositions)
		{
			if(covpos)
				CoverStruct.PositionsOfCoverPoints.Push(covpos->GetRelativeLocation());
			covpos->DestroyComponent();
		}
		if(CoverStruct.CoverPositions.Num()>=0) CoverStruct.CoverPositions.Empty();
		CoverStruct.CoverPositions.SetNum(CoverStruct.CoverPointsAmount);
		for(int i=0;i<CoverStruct.CoverPositions.Num();++i)
		{
			auto newCoverPoint=NewObject<UBoxComponent>(this,UBoxComponent::StaticClass(),*FString("CoverPos").Append(FString::FromInt(i+1)));
			newCoverPoint->CreationMethod=EComponentCreationMethod::Native;
			newCoverPoint->OnComponentCreated();
			newCoverPoint->SetupAttachment(RootComponent);
			newCoverPoint->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			newCoverPoint->SetMobility(EComponentMobility::Movable);
			if(i<CoverStruct.PositionsOfCoverPoints.Num())
			{
				newCoverPoint->SetWorldLocation(CoverStruct.PositionsOfCoverPoints[i]);
				newCoverPoint->SetRelativeLocation(CoverStruct.PositionsOfCoverPoints[i]);
			}
			newCoverPoint->RegisterComponent();
			CoverStruct.CoverPositions[i]=newCoverPoint;
		}
	}
}
#endif
