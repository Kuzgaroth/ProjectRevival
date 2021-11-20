// Project Revival. All Rights Reserved


#include "Miscellaneous/FuncActors/TickActor.h"

// Sets default values
ATickActor::ATickActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ATickActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATickActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnTick.Clear();
	//Timeline = FTimeline();
	Super::EndPlay(EndPlayReason);
}

void ATickActor::SetTimeline(FTimeline& Timeline)
{
	//this->Timeline = Timeline;
}

// Called every frame
void ATickActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OnTick.IsBound()) OnTick.Broadcast(DeltaTime);
	//if (Timeline.IsPlaying()) Timeline.TickTimeline(DeltaTime);
}

