// Project Revival. All Rights Reserved


#include "AI/AICoordinator.h"
#include "PlayerCharacter.h"
#include "StaticMeshAttributes.h"
#include "GameFramework/GameModeBase.h"
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
	
}


void AAICoordinator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AAICoordinator::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(PlayerInfoTimerHandle, this, &AAICoordinator::UpdateBotPlayerInfo, PlayerPositionUpdateTime, true);
}

void AAICoordinator::ProcessBotDeath(ASoldierAIController* BotController)
{
	if (BotMap.Num()==0) Destroy();
	BotMap.FindAndRemoveChecked(BotController);
	BotController->Destroy();
	
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
	for (UChildActorComponent* Component : PlayerStartComponents)
	{
		Component->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		Component->RegisterComponent();
	}
	int32 BotsInTotal = PlayerStartComponents.Num();
	int32 RightWing=0, LeftWing=0, Center=0;
	UE_LOG(LogPRCoordinator, Display, TEXT("Bots to spawn %d"), BotsInTotal);
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
	UE_LOG(LogPRCoordinator, Display, TEXT("Bots in LeftWing %d"), LeftWing);
	UE_LOG(LogPRCoordinator, Display, TEXT("Bots in RightWing %d"), RightWing);
	UE_LOG(LogPRCoordinator, Display, TEXT("Bots in Center %d"), Center);
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
	UE_LOG(LogPRCoordinator, Display, TEXT("Bots are left after target spawn %d"), AllSide);
	for (UChildActorComponent* PlayerStart : PlayerStartComponents)
	{
		if (LeftTmp>0)
		{
			SpawnBot(PlayerStart->GetChildActor(), EWing::Left);
			LeftTmp--;
		}
		else if (RightTmp>0)
		{
			SpawnBot(PlayerStart->GetChildActor(), EWing::Right);
			RightTmp--;
		}
		else if (CenterTmp>0)
		{
			SpawnBot(PlayerStart->GetChildActor(), EWing::Center);
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
		if (NumSide==0) break;
		SpawnBot(PlayerStartComponent->GetChildActor(), WingSide);
		NumSide--;
		NumAll--;
		AllComponents.Remove(PlayerStartComponent);
	}
}

void AAICoordinator::SpawnBot(AActor* PlayerStartActor, EWing WingSide)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const auto BotController = GetWorld()->SpawnActor<ASoldierAIController>(EnemyControllerClass, SpawnInfo);

	FRotator StartRotation(ForceInit);
	StartRotation.Yaw = PlayerStartActor->GetActorRotation().Yaw;
	FVector StartLocation = PlayerStartActor->GetActorLocation();
	FTransform Transform = FTransform(StartRotation, StartLocation);
	BotController->SetPawn(SpawnCharacterForBot(PlayerStartActor, Transform));

	ConnectController(BotController, WingSide);
	
	BotController->Possess(BotController->GetPawn());

	// If the Pawn is destroyed as part of possession we have to abort
	if (BotController->GetPawn() == nullptr)
	{
		BotController->FailedToSpawnPawn();
	}
	else
	{
		// Set initial control rotation to starting rotation rotation
		BotController->ClientSetRotation(BotController->GetPawn()->GetActorRotation(), true);

		FRotator NewControllerRot = StartRotation;
		NewControllerRot.Roll = 0.f;
		BotController->SetControlRotation(NewControllerRot);

		GetWorld()->GetAuthGameMode()->SetPlayerDefaults(BotController->GetPawn());
		
	}
	BotMap.Add(BotController, WingSide);
	PlayerStartActor->Destroy(true);
}

void AAICoordinator::ConnectController(ASoldierAIController* BotController, EWing WingSide)
{
	BotController->OnBotDied.AddUObject(this, &AAICoordinator::ProcessBotDeath);
	BotController->OnPlayerSpotted.AddUObject(this, &AAICoordinator::UpdatePlayerInfoFromBot);
	BotController->BotWing = WingSide;
	if (WingSide!=EWing::Center) BotController->OnBotWingDecision.BindUObject(this, &AAICoordinator::MakeDecisionForWingBot);
}

void AAICoordinator::UpdatePlayerInfoFromBot(FPlayerPositionData PlayerPos)
{
	this->PlayerPosition = PlayerPos;
}

bool AAICoordinator::MakeDecisionForWingBot() const
{
	if (BotMap.FilterByPredicate([](TTuple<ASoldierAIController*, EWing> BotPair)
	{
		if (BotPair.Value==EWing::Center) return true;
		return false;
	}).Num()>3) return true;
	return false;
}

void AAICoordinator::UpdateBotPlayerInfo()
{
	for (TTuple<ASoldierAIController*, EWing> BotPair : BotMap)
	{
		BotPair.Key->SetPlayerPos(PlayerPosition);
	}
}

AAICharacter* AAICoordinator::SpawnCharacterForBot(AActor* PlayerStartActor, const FTransform& Transform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = nullptr;
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	UClass* PawnClass = EnemyCharacterClass;
	return GetWorld()->SpawnActor<AAICharacter>(PawnClass, Transform, SpawnInfo);
}

void AAICoordinator::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                      bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		if (InitSpawn()) UE_LOG(LogPRCoordinator, Warning, TEXT("Bots are spawned"))
		else UE_LOG(LogPRCoordinator, Warning, TEXT("Bots are not spawned. See trouble in SpawnBots method."));
		TriggerComponent->OnComponentBeginOverlap.Clear();
	}
	
}

void AAICoordinator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(PlayerInfoTimerHandle);
}