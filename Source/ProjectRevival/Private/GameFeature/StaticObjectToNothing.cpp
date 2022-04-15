// Project Revival. All Rights Reserved


#include "GameFeature/StaticObjectToNothing.h"

#include <string>

#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystem/AbilityActors/ChangeWorldSphereActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"


// Sets default values
AStaticObjectToNothing::AStaticObjectToNothing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	


	InterpFunction.BindUFunction(this,FName("TimeLineFloatReturn"));
	OnTimeLineFinished.BindUFunction(this,FName("TimeLineFinished"));
	
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh"));
	RootComponent = SuperMesh;
	SuperMesh->SetMobility(EComponentMobility::Movable);
	SuperMesh->SetVisibility(true);
}

// Called when the game starts or when spawned
void AStaticObjectToNothing::BeginPlay()
{
	Super::BeginPlay();
	CollisionResponseContainer=SuperMesh->GetCollisionResponseToChannels();
	for(int i=0;i<this->Tags.Num();i++)
	{
		MeshTags.Add(this->Tags[i]);
	}
	SuperMesh->SetVisibility(true);
	int32 num=SuperMesh->GetNumMaterials();
	for (int32 i=0;i<num;i++)
	{
		const auto Material = SuperMesh->CreateDynamicMaterialInstance(i);
		MeshesMaterials.Add(Material);
	}
	if(VisualCurve)
	{
		VisualCurve->GetValueRange(MinCurveValue,MaxCurveValue);
		TimeLine.AddInterpFloat(VisualCurve,InterpFunction);
		TimeLine.SetTimelineFinishedFunc(OnTimeLineFinished);
		TimeLine.SetLooping(false);
	}
	if (World==OrdinaryWorld)
	{
		isApearing=true;
		if(VisualCurve&&MeshesMaterials.Num()>0)
		{
			for (const auto Material : MeshesMaterials)
			{
				if (Material!=nullptr)
				{
					Material->SetScalarParameterValue("Amount",MinCurveValue);
					Material->SetVectorParameterValue("Color",FLinearColor::Blue);
				}
			}
		}
		else
		{
			SuperMesh->SetVisibility(true);
		}
		SuperMesh->SetCollisionProfileName("OverlapAll");
		SuperMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
	}
	else
	{
		if(VisualCurve&&MeshesMaterials.Num()>0)
		{
			for (const auto Material : MeshesMaterials)
			{
				if (Material!=nullptr)
				{
					Material->SetScalarParameterValue("Amount",MaxCurveValue);
					Material->SetVectorParameterValue("Color",FLinearColor::Red);
				}
			}
		}
		else
		{
			SuperMesh->SetVisibility(false);
		}
		SuperMesh->SetCollisionProfileName("OverlapAll");
		ClearComponentTags(SuperMesh);
	}
	SuperMesh->OnComponentBeginOverlap.AddDynamic(this,&AStaticObjectToNothing::OnMeshComponentCollision);

	SuperMesh->GetChildrenComponents(true,CoverStruct.CoverPositions);
	
	for(auto covpoint:CoverStruct.CoverPositions)
	{
		auto boxcomp=Cast<UBoxComponent>(covpoint);
		if(boxcomp)
		{
			CoverStruct.PointIsNotTaken.Add(boxcomp,true);
			boxcomp->OnComponentBeginOverlap.AddDynamic(this,&AStaticObjectToNothing::OnCoverPointComponentCollision);
			boxcomp->OnComponentEndOverlap.AddDynamic(this,&AStaticObjectToNothing::OnCoverPointComponentExit);
			
		}
	}
	SuperMesh->OnComponentEndOverlap.AddDynamic(this,&AStaticObjectToNothing::OnMeshComponentEndCollision);
}

// Called every frame
void AStaticObjectToNothing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(TimeLine.IsPlaying())
	{
		TimeLine.TickTimeline(DeltaTime);
	}
}
#if WITH_EDITOR
void AStaticObjectToNothing::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(PropertyChangedEvent.Property->GetName()=="VisibleWorld")
	{
		if(VisibleWorld==World || VisibleWorld==BothWorlds)
		{
			SuperMesh->SetVisibility(true);
		}
		else
		{
			SuperMesh->SetVisibility(false);
		}
	}
	if(PropertyChangedEvent.Property->GetName()=="AllObjectVisibleWorld")
	{
		TArray<AActor*> ChangeAbleObjs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),AChangeWorld::StaticClass(),ChangeAbleObjs);
		for(auto obj:ChangeAbleObjs)
		{
			auto chobj=Cast<AChangeWorld>(obj);
			chobj->ChangeVisibleWorld(AllObjectVisibleWorld);
		}
	}
}
#endif


void AStaticObjectToNothing::Changing()
{
	if(CurrentWorld==OrdinaryWorld)
	{
		CurrentWorld=OtherWorld;
	}
	else
	{
		CurrentWorld=OrdinaryWorld;
	}
	if (World == CurrentWorld)
	{
		if(VisualCurve)
		{
			isApearing=true;
			SuperMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
			LoadComponentTags(SuperMesh);
			TimeLine.PlayFromStart();
			
		}
		else
		{
			SuperMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
			LoadComponentTags(SuperMesh);
			SuperMesh->SetVisibility(true);
		}
	}
	else
	{
		if(VisualCurve)
		{
			isApearing=false;

			TimeLine.PlayFromStart();

		}
		else
		{
			ClearComponentTags(SuperMesh);
			SuperMesh->SetCollisionProfileName("OverlapAll");
			SuperMesh->SetVisibility(false);
		}
	}
}

void AStaticObjectToNothing::ChangeVisibleWorld(EChangeAllMapEditorVisibility VisibleInEditorWorld)
{
	Super::ChangeVisibleWorld(VisibleInEditorWorld);
	if(VisibleInEditorWorld!=OwnValuesWorld)
	{
		switch (VisibleInEditorWorld)
		{
		case DefaultVisibleWorld:
			VisibleWorld=DefaultWorld;
			break;
		case OtherVisibleWorld:
			VisibleWorld=AltirnativeWorld;
			break;
		case BothVisibleWorlds:
			VisibleWorld=BothWorlds;
			break;
		default:
			break;
		}
		if(VisibleWorld==World || VisibleWorld==BothWorlds)
		{
			SuperMesh->SetVisibility(true);
		}
		else
		{
			SuperMesh->SetVisibility(false);
		}
	}
	else
	{
		VisibleWorld=DefaultWorld;
		if(VisibleWorld==World || VisibleWorld==BothWorlds)
		{
			SuperMesh->SetVisibility(true);
		}
		else
		{
			SuperMesh->SetVisibility(false);
		}
	}
}

void AStaticObjectToNothing::ShowChangeWorldObjectByAbility()
{
	SuperMesh->SetRenderCustomDepth(true);
	if(SuperMesh->GetCollisionProfileName()=="OverlapAll")
	{
		if(MeshesMaterials.Num()!=0)
			for (const auto Material : MeshesMaterials)
			{
				auto reqwar=(MaxCurveValue-MinCurveValue)/TransparencyLevel;
				reqwar=MaxCurveValue-reqwar;
				if (Material!=nullptr)
				Material->SetScalarParameterValue("Amount",reqwar);
			}
	}
	
}

void AStaticObjectToNothing::HideChangeWorldObjectByAbility()
{
	SuperMesh->SetRenderCustomDepth(false);
	if(SuperMesh->GetCollisionProfileName()=="OverlapAll")
	{
		if(MeshesMaterials.Num()!=0)
			for (const auto Material : MeshesMaterials)
			{
				if (Material!=nullptr)
				Material->SetScalarParameterValue("Amount",MaxCurveValue);
			}
	}
}

void AStaticObjectToNothing::OnMeshComponentCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<AChangeWorldSphereActor>(OtherActor))
	{
		Changing();
	}
	auto Player=Cast<APlayerCharacter>(OtherActor);
	if(Player&& SuperMesh->GetCollisionProfileName()=="OverlapAll")
	{
		Player->SetChangeWorldPossibility(false,this);
		
	}
}

void AStaticObjectToNothing::OnMeshComponentEndCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Player=Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		Player->SetChangeWorldPossibility(true,nullptr);
		HideChangeWorldObjectByAbility();
	}
}

void AStaticObjectToNothing::OnCoverPointComponentCollision(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,
															UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult)
{
	if(Cast<APawn>(OtherActor))
	{
		BlockCoverPoint(Cast<UBoxComponent>(OverlappedComponent));
	}
}

void AStaticObjectToNothing::OnCoverPointComponentExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Cast<APawn>(OtherActor))
	{
		FreeCoverPoint(Cast<UBoxComponent>(OverlappedComponent));
	}
}


void AStaticObjectToNothing::BlockCoverPoint(const UBoxComponent* CoverPoint)
{
	CoverStruct.PointIsNotTaken[CoverPoint]=false;
}

void AStaticObjectToNothing::FreeCoverPoint(const UBoxComponent* CoverPoint)
{
	CoverStruct.PointIsNotTaken[CoverPoint]=true;
}

void AStaticObjectToNothing::TimeLineFinished()
{
	if(!isApearing)
	{
		SuperMesh->SetCollisionProfileName("OverlapAll");
		ClearComponentTags(SuperMesh);

	}
}

void AStaticObjectToNothing::TimeLineFloatReturn(float Value)
{
	for (const auto Material : MeshesMaterials)
	{
		if(isApearing)
		{
			if (Material!=nullptr)
			Material->SetScalarParameterValue("Amount",Value);
		}
		else
		{
			float val=MinCurveValue-Value;
			val=MaxCurveValue+val;
			if (Material!=nullptr)
			Material->SetScalarParameterValue("Amount",val);
		}
	}
	
}



void AStaticObjectToNothing::LoadComponentTags(UStaticMeshComponent* supermesh)
{
	Super::LoadComponentTags(supermesh);
	for(int i=0;i<MeshTags.Num();i++)
	{
		this->Tags.AddUnique(MeshTags[i]);
	}
}

bool AStaticObjectToNothing::CheckIsChangeAbleObjIsCover()
{
	return CoverStruct.CanBeTakenAsCover&&this->ActorHasTag("Cover");
}

bool AStaticObjectToNothing::TryToFindCoverPoint(FVector PlayerPos, FVector& CoverPos)
{
	if(CoverStruct.CoverPositions.Num()==0) return false;
	for(auto covpos:CoverStruct.CoverPositions)
	{
		FVector TraceStart=covpos->GetComponentLocation();
		FVector TraceEnd=PlayerPos;
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,ECollisionChannel::ECC_Visibility,CollisionParams);
		auto box =Cast<UBoxComponent>(covpos);
		if(HitResult.bBlockingHit)
		{
			DrawDebugLine(GetWorld(),TraceStart,HitResult.ImpactPoint,FColor::Blue,false,3.0f,0,3.0f);
			if(HitResult.Actor==this&& CoverStruct.PointIsNotTaken.Contains(box)&&CoverStruct.PointIsNotTaken[box])
			{
				CoverStruct.PointIsNotTaken[box]=false;
				CoverPos=TraceStart;
				return true;
			}
		}
	}
	return false;
}
