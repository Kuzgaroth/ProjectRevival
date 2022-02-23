// Project Revival. All Rights Reserved


#include "GameFeature/StaticObjectToStaticObject.h"
#include "Components/BoxComponent.h"

// Sets default values
AStaticObjectToStaticObject::AStaticObjectToStaticObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SceneComponent->SetMobility(EComponentMobility::Static);
	RootComponent = SceneComponent;

	SuperMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh1"));
	SuperMesh1->SetupAttachment(RootComponent);
	SuperMesh1->SetMobility(EComponentMobility::Movable);
	SuperMesh1->SetVisibility(true);

	SuperMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh2"));
	SuperMesh2->SetupAttachment(RootComponent);
	SuperMesh2->SetMobility(EComponentMobility::Movable);
	SuperMesh2->SetVisibility(true);
}

// Called when the game starts or when spawned
void AStaticObjectToStaticObject::BeginPlay()
{
	Super::BeginPlay();
	SuperMesh1->OnComponentBeginOverlap.AddDynamic(this,&AStaticObjectToStaticObject::OnOrdinaryMeshCollision);
	OrdinaryWorldCollisionResponseContainer=SuperMesh1->GetCollisionResponseToChannels();
	SuperMesh2->OnComponentBeginOverlap.AddDynamic(this,&AStaticObjectToStaticObject::OnOtherMeshCollision);
	OtherWorldCollisionResponseContainer=SuperMesh2->GetCollisionResponseToChannels();
	int32 num=SuperMesh1->GetNumMaterials();
	for (int32 i=0;i<num;i++)
	{
		const auto Material = SuperMesh1->CreateDynamicMaterialInstance(i);
		OrdinaryWMeshesMaterials.Add(Material);
	}
	num=SuperMesh2->GetNumMaterials();
	for (int32 i=0;i<num;i++)
	{
		const auto Material = SuperMesh2->CreateDynamicMaterialInstance(i);
		OtherWMeshesMaterials.Add(Material);
	}
	if(OrdinaryWVisualCurve&&OtherWVisualCurve)
	{
		OrdinaryWVisualCurve->GetValueRange(MinOrWValue,MaxOrWValue);
		OtherWVisualCurve->GetValueRange(MinOtWValue,MaxOtValue);
		OrdinaryWTimeLine.AddInterpFloat(OrdinaryWVisualCurve,OrWInterpFunction);
		OtherWTimeLine.AddInterpFloat(OtherWVisualCurve,OtWInterpFunction);
		OrdinaryWTimeLine.SetTimelineFinishedFunc(OrOnTimeLineFinished);
		OtherWTimeLine.SetTimelineFinishedFunc(OtOnTimeLineFinished);
		OrdinaryWTimeLine.SetLooping(false);
		OtherWTimeLine.SetLooping(false);
	}
	if (CurrentWorld==OrdinaryWorld)
	{
		if(OrdinaryWVisualCurve&&OtherWVisualCurve)
		{
			for (const auto Material : OrdinaryWMeshesMaterials)
			{
				Material->SetScalarParameterValue("Amount",MinOrWValue);
			}
			for (const auto Material : OtherWMeshesMaterials)
			{
				Material->SetScalarParameterValue("Amount",MaxOtValue);
			}
		}
		else
		{
			SuperMesh1->SetCollisionProfileName("BlockAll");
			SuperMesh1->SetVisibility(true);
			SuperMesh2->SetCollisionProfileName("OverlapAll");
			SuperMesh2->SetVisibility(false);
		}
		SuperMesh1->SetCollisionProfileName("OverlapAll");
		SuperMesh1->SetCollisionResponseToChannels(OrdinaryWorldCollisionResponseContainer);
		SuperMesh2->SetCollisionProfileName("OverlapAll");
	}
	else
	{
		if(OrdinaryWVisualCurve&&OtherWVisualCurve)
		{
			for (const auto Material : OrdinaryWMeshesMaterials)
			{
				Material->SetScalarParameterValue("Amount",MaxOrWValue);
			}
			for (const auto Material : OtherWMeshesMaterials)
			{
				Material->SetScalarParameterValue("Amount",MinOtWValue);
			}
		}
		else
		{
			SuperMesh1->SetCollisionProfileName("OverlapAll");
			SuperMesh1->SetVisibility(false);
			SuperMesh2->SetCollisionProfileName("BlockAll");
			SuperMesh2->SetVisibility(true);
		}
		SuperMesh2->SetCollisionProfileName("OverlapAll");
		SuperMesh2->SetCollisionResponseToChannels(OrdinaryWorldCollisionResponseContainer);
		SuperMesh1->SetCollisionProfileName("OverlapAll");
	}
}

// Called every frame
void AStaticObjectToStaticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(OrdinaryWTimeLine.IsPlaying())
	{
		OrdinaryWTimeLine.TickTimeline(DeltaTime);
	}
	if(OtherWTimeLine.IsPlaying())
	{
		OtherWTimeLine.TickTimeline(DeltaTime);
	}

}
#if WITH_EDITOR
void AStaticObjectToStaticObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(VisibleWorld==DefaultWorld)
	{
		SuperMesh1->SetVisibility(true);
		SuperMesh2->SetVisibility(false);

	}
	else if(VisibleWorld==AltirnativeWorld)
	{
		SuperMesh1->SetVisibility(false);
		SuperMesh2->SetVisibility(true);
	}
	else
	{
		SuperMesh1->SetVisibility(true);
		SuperMesh2->SetVisibility(true);
	}
}
#endif


void AStaticObjectToStaticObject::OnOrdinaryMeshCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AStaticObjectToStaticObject::OnOtherMeshCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AStaticObjectToStaticObject::OrdinaryWTimelineFinished()
{
}

void AStaticObjectToStaticObject::OtherWTimelineFinished()
{
}

void AStaticObjectToStaticObject::OrdinaryWTimelineFloatReturn(float Value)
{
}

void AStaticObjectToStaticObject::OtherWTimelineFloatReturn(float Value)
{
}





void AStaticObjectToStaticObject::Changing()
{
	if(CurrentWorld==OrdinaryWorld)
	{
		CurrentWorld=OtherWorld;
		SuperMesh1->SetCollisionProfileName("OverlapAll");
		SuperMesh1->SetVisibility(false);
		SuperMesh2->SetCollisionProfileName("BlockAll");
		SuperMesh2->SetVisibility(true);
	}
	else
	{
		CurrentWorld=OrdinaryWorld;
        SuperMesh1->SetCollisionProfileName("BlockAll");
        SuperMesh1->SetVisibility(true);
        SuperMesh2->SetCollisionProfileName("OverlapAll");
        SuperMesh2->SetVisibility(false);
	}
	
}
