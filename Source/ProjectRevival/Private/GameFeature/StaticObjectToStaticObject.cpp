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

	CollisionComponent1 = CreateDefaultSubobject<UBoxComponent>(FName("CollisionComponent1"));
	CollisionComponent1->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	CollisionComponent1->bDynamicObstacle = true;
	CollisionComponent1->SetupAttachment(RootComponent);
	CollisionComponent1->SetMobility(EComponentMobility::Static);
	CollisionComponent1->SetCollisionProfileName("BlockAll");

	CollisionComponent2 = CreateDefaultSubobject<UBoxComponent>(FName("CollisionComponent2"));
	CollisionComponent2->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	CollisionComponent2->bDynamicObstacle = true;
	CollisionComponent2->SetupAttachment(RootComponent);
	CollisionComponent2->SetMobility(EComponentMobility::Static);
	CollisionComponent2->SetCollisionProfileName("OverlapAll");

	SuperMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh1"));
	SuperMesh1->SetupAttachment(CollisionComponent1);
	SuperMesh1->SetMobility(EComponentMobility::Movable);

	SuperMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh2"));
	SuperMesh2->SetupAttachment(CollisionComponent2);
	SuperMesh2->SetMobility(EComponentMobility::Movable);
	SuperMesh2->SetVisibility(false);
}

// Called when the game starts or when spawned
void AStaticObjectToStaticObject::BeginPlay()
{
	Super::BeginPlay();

	if (World != CurrentWorld)
	{
		CollisionComponent1->SetCollisionProfileName("OverlapAll");
		SuperMesh1->SetVisibility(false);
		CollisionComponent2->SetCollisionProfileName("BlockAll");
		SuperMesh2->SetVisibility(true);
	}
}

// Called every frame
void AStaticObjectToStaticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStaticObjectToStaticObject::Changing()
{
	if (World == CurrentWorld)
	{
		if (CurrentWorld == OrdinaryWorld) CurrentWorld = OtherWorld; else CurrentWorld = OrdinaryWorld;
		CollisionComponent1->SetCollisionProfileName("OverlapAll");
		SuperMesh1->SetVisibility(false);
		CollisionComponent2->SetCollisionProfileName("BlockAll");
		SuperMesh2->SetVisibility(true);
	}
	else
	{
		CurrentWorld = World;
		CollisionComponent1->SetCollisionProfileName("BlockAll");
		SuperMesh1->SetVisibility(true);
		CollisionComponent2->SetCollisionProfileName("OverlapAll");
		SuperMesh2->SetVisibility(false);
	}
}