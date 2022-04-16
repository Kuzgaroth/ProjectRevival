// Project Revival. All Rights Reserved


#include "CourtLair/LeaveLairActor.h"
#include "Kismet/GameplayStatics.h"

ALeaveLairActor::ALeaveLairActor()
{
	PrimaryActorTick.bCanEverTick = false;
	TriggerComponent = CreateDefaultSubobject<UBoxComponent>("TriggerComponent");
	TriggerComponent->SetupAttachment(RootComponent);
}

void ALeaveLairActor::BeginPlay()
{
	Super::BeginPlay();
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ALeaveLairActor::OnTriggerLeaveComponent);
}

void ALeaveLairActor::OnTriggerLeaveComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerComponent->OnComponentBeginOverlap.Clear();
	UGameplayStatics::OpenLevel(this,/*GameInstance->GetStartupLevel().LevelName*/"LVL_Conference",true);
}


