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
	
	PlayerStartComponent->SetupAttachment(RootComponent);
	
}

void ACheckpointActor::BeginPlay()
{
	Super::BeginPlay();
	if (!IsFirstCheckPoint())
		TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointActor::OnTriggerSave);
}

void ACheckpointActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACheckpointActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PlayerStartComponent->SetChildActorClass(APlayerStart::StaticClass());
}

AActor* ACheckpointActor::GetPlayerStartForCheckpoint()
{
	return PlayerStartComponent->GetChildActor();
}

bool ACheckpointActor::HasName(FName Name)
{
	return Name==this->CheckpointName;
}

bool ACheckpointActor::IsFirstCheckpointOnMap()
{
	return IsFirstCheckPoint();
}

void ACheckpointActor::OnTriggerSave(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto GameMode = GetWorld()->GetAuthGameMode<APRGameModeBase>();
	if (!GameMode) return;
	GameMode->WriteSaveGame(CheckpointName);
	
	TriggerComponent->OnComponentBeginOverlap.Clear();
	Destroy();
}

