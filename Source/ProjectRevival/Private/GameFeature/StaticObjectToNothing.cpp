// Project Revival. All Rights Reserved


#include "GameFeature/StaticObjectToNothing.h"

#include <string>

#include "AbilitySystem/Abilities/Miscellaneuos/IDynMaterialsFromMesh.h"
#include "AbilitySystem/AbilityActors/ChangeWorldSphereActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameUserSettings.h"

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
	for(int i=0;i<SuperMesh->ComponentTags.Num();i++)
	{
		MeshTags.Add(SuperMesh->ComponentTags[i]);
	}
	SuperMesh->SetVisibility(true);
	//TArray<USceneComponent*> components;
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
		if(VisualCurve)
		{
			for (const auto Material : MeshesMaterials)
			{
				Material->SetScalarParameterValue("Amount",MinCurveValue);
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
		if(VisualCurve)
		{
			for (const auto Material : MeshesMaterials)
			{
				Material->SetScalarParameterValue("Amount",MaxCurveValue);
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
			SuperMesh->SetVisibility(true);
		}
	}
	else
	{
		//CurrentWorld = World;
		if(VisualCurve)
		{
			isApearing=false;
			SuperMesh->SetCollisionProfileName("OverlapAll");
			TimeLine.PlayFromStart();

		}
		else
		{
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

void AStaticObjectToNothing::OnMeshComponentCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<AChangeWorldSphereActor>(OtherActor))
	{
		Changing();
	}
}

void AStaticObjectToNothing::TimeLineFinished()
{
	if(!isApearing)
	{
		ClearComponentTags(SuperMesh);

	}
}

void AStaticObjectToNothing::TimeLineFloatReturn(float Value)
{
	for (const auto Material : MeshesMaterials)
	{
		if(isApearing)
		{
			Material->SetScalarParameterValue("Amount",Value);
		}
		else
		{
			float val=MinCurveValue-Value;
			val=MaxCurveValue+val;
			Material->SetScalarParameterValue("Amount",val);
		}
	}
	
}



void AStaticObjectToNothing::LoadComponentTags(UStaticMeshComponent* supermesh)
{
	Super::LoadComponentTags(supermesh);
	for(int i=0;i<MeshTags.Num();i++)
	{
		supermesh->ComponentTags.AddUnique(MeshTags[i]);
	}
}

bool AStaticObjectToNothing::CheckIsChangeAbleObjIsCover()
{
	return SuperMesh->ComponentTags.Contains("Cover");
}
