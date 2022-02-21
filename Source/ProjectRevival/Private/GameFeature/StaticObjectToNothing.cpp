// Project Revival. All Rights Reserved


#include "GameFeature/StaticObjectToNothing.h"

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
	
	
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh"));
	SuperMesh->SetupAttachment(RootComponent);
	SuperMesh->SetMobility(EComponentMobility::Movable);
	SuperMesh->SetVisibility(true);
}

// Called when the game starts or when spawned
void AStaticObjectToNothing::BeginPlay()
{
	Super::BeginPlay();
	
	if (World==OrdinaryWorld)
	{
		SuperMesh->SetVisibility(true);
		SuperMesh->SetCollisionProfileName("BlockAll");
		//SuperMesh->SetCollisionResponseToChannel(, ECollisionResponse::ECR_Overlap)
		//SetActorEnableCollision(false);
		//SetActorHiddenInGame(true);
		
	}
	else
	{
		SuperMesh->SetVisibility(false);
		SuperMesh->SetCollisionProfileName("OverlapAll");
	}
}

// Called every frame
void AStaticObjectToNothing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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
		//if (CurrentWorld == OrdinaryWorld) CurrentWorld = OtherWorld; else CurrentWorld = OrdinaryWorld;
		
		GLog->Log("Overlaping");
		//if(VisualCurve)
		//{
			GLog->Log("GoingHere");
			FOnTimelineFloat TimeLineProgress;
			
			//MeshesMaterials= Cast<IIDynMaterialsFromMesh>(SuperMesh)->GetDynMaterials();
			TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
			TimeLine.AddInterpFloat(VisualCurve,TimeLineProgress);
			TimeLine.SetLooping(false);
			OnAppearFinished.BindUFunction(this,FName("OnTimeLineFinished"));
			TimeLine.SetTimelineFinishedFunc(OnAppearFinished);
		//}

		//SetActorHiddenInGame(true);
	}
	else
	{
		//CurrentWorld = World;
		SuperMesh->SetCollisionProfileName("OverlapAll");
		SuperMesh->SetVisibility(false);
		//SetActorEnableCollision(true);
		//SetActorHiddenInGame(false);
	}
}

void AStaticObjectToNothing::OnTimeLineFinished()
{
	GLog->Log("Timeline working");
	SuperMesh->SetCollisionProfileName("BlockAll");
	SuperMesh->SetVisibility(true);
}
