// Project Revival. All Rights Reserved


#include "GameFeature/CoverObject.h"

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
