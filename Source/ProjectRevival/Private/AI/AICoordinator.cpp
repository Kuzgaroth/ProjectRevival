// Project Revival. All Rights Reserved


#include "AI/AICoordinator.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerStart.h"

DEFINE_LOG_CATEGORY(LogPRCoordinator)

// Sets default values
AAICoordinator::AAICoordinator()
{
	PrimaryActorTick.bCanEverTick = false;

	ArenaComponent = CreateDefaultSubobject<UBoxComponent>("ArenaSpace");
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	ArenaComponent->SetupAttachment(RootComponent);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>("FightTrigger");
	TriggerComponent->SetupAttachment(RootComponent);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AAICoordinator::OnTriggerOverlap);
	OnBotDied.AddUObject(this, &AAICoordinator::ProcessBotDeath);
	
}


void AAICoordinator::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAICoordinator::ProcessBotDeath(AAIController* BotController)
{
	
}

bool AAICoordinator::InitSpawn()
{
	TArray<UChildActorComponent*> ChildComponents;
	GetComponents(ChildComponents);
	if (ChildComponents.Num()<1) return false;
	auto PlayerStartComponents = ChildComponents.FilterByPredicate([](const UChildActorComponent* Child)
	{
		if (Child->GetChildActorClass()==APlayerStart::StaticClass()) return true;
		return false;
	});
	if (PlayerStartComponents.Num()<1) return false;
	int32 BotsInTotal = PlayerStartComponents.Num();
	int32 RightWing=0, LeftWing=0, Center=0;
	if (BotsInTotal>3)
	{
		RightWing = LeftWing = FMath::RoundToInt(static_cast<float>(BotsInTotal)/4);
		Center = BotsInTotal-2*RightWing;
	}
	else if (BotsInTotal==3)
	{
		RightWing=LeftWing=Center=1;
	}
	else Center = BotsInTotal;
	auto LeftWingStarts = PlayerStartComponents.FilterByPredicate([](const UChildActorComponent* PlayerStartComponent)
	{
		return PlayerStartComponent->ComponentTags.Find("Left") !=INDEX_NONE;
	});
	auto RightWingStarts = PlayerStartComponents.FilterByPredicate([](const UChildActorComponent* PlayerStartComponent)
	{
		return PlayerStartComponent->ComponentTags.Find("Right") !=INDEX_NONE;
	});
	auto CenterWingStarts = PlayerStartComponents.FilterByPredicate([](const UChildActorComponent* PlayerStartComponent)
	{
		return PlayerStartComponent->ComponentTags.Find("Center") !=INDEX_NONE;
	});
	int32 LeftTmp=LeftWing, RightTmp = RightWing, CenterTmp = Center, AllSide = BotsInTotal;
	SpawnBotsBySide(LeftWingStarts, PlayerStartComponents, LeftTmp, AllSide, EWing::Left);
	SpawnBotsBySide(RightWingStarts, PlayerStartComponents, RightTmp, AllSide, EWing::Right);
	SpawnBotsBySide(CenterWingStarts, PlayerStartComponents, CenterTmp, AllSide, EWing::Center);
	for (UChildActorComponent* PlayerStart : PlayerStartComponents)
	{
		if (LeftTmp>0)
		{
			SpawnBot(PlayerStart->GetChildActor()->GetTransform(), EWing::Left);
			LeftTmp--;
		}
		else if (RightTmp>0)
		{
			SpawnBot(PlayerStart->GetChildActor()->GetTransform(), EWing::Right);
			RightTmp--;
		}
		else if (CenterTmp>0)
		{
			SpawnBot(PlayerStart->GetChildActor()->GetTransform(), EWing::Center);
			CenterTmp--;
		}
		
	}
	return true;
}

void AAICoordinator::SpawnBotsBySide(TArray<UChildActorComponent*>& SideComponents,
	TArray<UChildActorComponent*>& AllComponents, int32& NumSide, int32& NumAll, EWing WingSide)
{
	for (UChildActorComponent* PlayerStartComponent : SideComponents)
	{
		SpawnBot(PlayerStartComponent->GetChildActor()->GetTransform(), WingSide);
		if (NumSide==0) break;
		else
		{
			NumSide--;
			NumAll--;
			AllComponents.Remove(PlayerStartComponent);
		}
	}
}

void AAICoordinator::SpawnBot(const FTransform& SpawnTransform, EWing WingSide)
{
	
}

void AAICoordinator::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                      bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		if (InitSpawn()) UE_LOG(LogPRCoordinator, Display, TEXT("Bots are spawned"))
		else UE_LOG(LogPRCoordinator, Display, TEXT("Bots are not spawned. See trouble in SpawnBots method."));
	}
}