// Project Revival. All Rights Reserved


#include "GameFeature/ChangeWorld.h"

// Sets default values
AChangeWorld::AChangeWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChangeWorld::BeginPlay()
{
	Super::BeginPlay();

}

void AChangeWorld::ClearComponentTags(UStaticMeshComponent* supermesh)
{
	this->Tags.Empty();
}

void AChangeWorld::LoadComponentTags(UStaticMeshComponent* supermesh)
{
}

// Called every frame
void AChangeWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AChangeWorld::ChangeVisibleWorld(EChangeAllMapEditorVisibility VisibleInEditorWorld)
{
	
}

void AChangeWorld::Changing()
{
	
}

