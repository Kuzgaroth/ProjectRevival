// Project Revival. All Rights Reserved


#include "SaveSystem/CheckpointActor.h"

#include "PRGameModeBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerStart.h"

ACheckpointActor::ACheckpointActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	TriggerComponent = CreateDefaultSubobject<UBoxComponent>("SaveTrigger");
	TriggerComponent->SetupAttachment(RootComponent);
	PlayerStartComponent = CreateDefaultSubobject<UChildActorComponent>("PlayerStartPoint");
	PlayerStartComponent->SetChildActorClass(APlayerStart::StaticClass());
	PlayerStartComponent->SetupAttachment(RootComponent);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointActor::OnTriggerSave);
}

void ACheckpointActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACheckpointActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	Super::EndPlay(EndPlayReason);
}

void ACheckpointActor::OnTriggerSave(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto GameMode = GetWorld()->GetAuthGameMode<APRGameModeBase>();
	if (!GameMode) return;
	
	TriggerComponent->OnComponentBeginOverlap.Clear();
}

