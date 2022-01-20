// Project Revival. All Rights Reserved


#include "GameFeature/StaticObjectToNothing.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
AStaticObjectToNothing::AStaticObjectToNothing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SceneComponent->SetMobility(EComponentMobility::Static);
	RootComponent = SceneComponent;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(FName("CollisionMesh"));
	CollisionComponent->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	CollisionComponent->bDynamicObstacle = true;
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetMobility(EComponentMobility::Static);
	CollisionComponent->SetCollisionProfileName("BlockAll");
	
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh"));
	SuperMesh->SetupAttachment(CollisionComponent);
	SuperMesh->SetMobility(EComponentMobility::Movable);
	
}

// Called when the game starts or when spawned
void AStaticObjectToNothing::BeginPlay()
{
	Super::BeginPlay();
	
	if (World != CurrentWorld)
	{
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
	}
}

// Called every frame
void AStaticObjectToNothing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void AStaticObjectToNothing::Changing()
{
	if (World == CurrentWorld)
	{
		if (CurrentWorld == OrdinaryWorld) CurrentWorld = OtherWorld; else CurrentWorld = OrdinaryWorld;
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
	}
	else
	{
		CurrentWorld = World;
		SetActorEnableCollision(true);
		SetActorHiddenInGame(false);
	}
}