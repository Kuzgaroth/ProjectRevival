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

// Called every frame
void AChangeWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AChangeWorld::TimelineProgress(float Value)
{
	//for (const auto Material : MeshesMaterials)
	//{
	//	Material->SetScalarParameterValue("Amount",Value);
	//}
	GLog->Log("Progressing");
}
void AChangeWorld::OnTimeLineFinished()
{
	
}
void AChangeWorld::Changing()
{
	
}

