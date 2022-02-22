// Project Revival. All Rights Reserved


#include "GameFeature/StaticObjectToNothing.h"

#include <string>

#include "AbilitySystem/Abilities/Miscellaneuos/IDynMaterialsFromMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameUserSettings.h"

// Sets default values
AStaticObjectToNothing::AStaticObjectToNothing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SceneComponent->SetMobility(EComponentMobility::Static);
	RootComponent = SceneComponent;

	InterpFunction.BindUFunction(this,FName("TimeLineFloatReturn"));
	OnTimeLineFinished.BindUFunction(this,FName("TimeLineFinished"));	
	OnTimeLineStart.BindUFunction(this,FName(""));
	
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh"));
	SuperMesh->SetupAttachment(RootComponent);
	SuperMesh->SetMobility(EComponentMobility::Movable);
	SuperMesh->SetVisibility(true);
}

// Called when the game starts or when spawned
void AStaticObjectToNothing::BeginPlay()
{
	Super::BeginPlay();
	CollisionResponseContainer=SuperMesh->GetCollisionResponseToChannels();
	auto components=this->GetComponents();
	
	//const auto f=Cast<UMeshComponent>(components);
	for(auto obj :components)
	{
		auto mat=Cast<UMeshComponent>(obj);
		if(mat)
		{
			int32 num=mat->GetNumMaterials();
			for (int32 i=0;i<num;i++)
			{
				const auto Material = mat->CreateDynamicMaterialInstance(i);
				MeshesMaterials.Add(Material);
			}
		}
	}
	if(VisualCurve)
	{
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
				Material->SetScalarParameterValue("Amount",-MinCurveValue);
			}
			TimeLine.SetNewTime(TimeLine.GetTimelineLength());
		}
		else
		{
			SuperMesh->SetVisibility(true);
		}
		SuperMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
	}
	else
	{
		if(VisualCurve)
		{
			//SuperMesh->SetVisibility(false);
			for (const auto Material : MeshesMaterials)
			{
				Material->SetScalarParameterValue("Amount",MaxCureveValue);
				//Material->SetScalarParameterValue("Amount",VisualCurve->);
			}
		}
		else
		{
			SuperMesh->SetVisibility(false);
		}
		SuperMesh->SetCollisionProfileName("OverlapAll");
	}
	
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
	if(VisibleWorld==World || VisibleWorld==BothWorlds)
	{
		SuperMesh->SetVisibility(true);
	}
	else
	{
		SuperMesh->SetVisibility(false);
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
			TimeLine.PlayFromStart();
		}
		else
		{
			SuperMesh->SetVisibility(false);
		}
	}
}

void AStaticObjectToNothing::TimeLineFinished()
{
	GLog->Log("Timeline working");
	if(SuperMesh->GetCollisionResponseToChannels()==CollisionResponseContainer)
	{
		SuperMesh->SetCollisionProfileName("OverlapAll");
	}
	else
	{
		SuperMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
	}
}

void AStaticObjectToNothing::TimeLineFloatReturn(float Value)
{
	Super::TimeLineFloatReturn(Value);
	for (const auto Material : MeshesMaterials)
	{
		GLog->Log(FString::SanitizeFloat(-Value));
		//GLog->Log(FString::SanitizeFloat(-Value));
		if(isApearing)
		{
			GLog->Log("IsApearing");
			Material->SetScalarParameterValue("Amount",-Value);
		}
		else
		{
			float val=MinCurveValue-Value;
			val=MaxCureveValue+val;
			GLog->Log(FString::SanitizeFloat(-val));
			Material->SetScalarParameterValue("Amount",-val);
		}
	}
	
}
