// Project Revival. All Rights Reserved


#include "GameFeature/PatrolPathActor.h"
#include "Algo/Reverse.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY(LogPRPatrolPath);

// Sets default values
APatrolPathActor::APatrolPathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PatrolPath.MaxNumOfPatrollingBots = 0;
	PatrolPath.CurrentNumOfPatrollingBots = 0;
	
	PatrolPath.bCanBeTaken = true;
	PatrolPath.bIsReversed = false;
}

bool APatrolPathActor::AddPatrollingBot()
{
	if (PatrolPath.CurrentNumOfPatrollingBots < PatrolPath.MaxNumOfPatrollingBots)
	{
		PatrolPath.CurrentNumOfPatrollingBots++;
		UE_LOG(LogPRPatrolPath, Log, TEXT("%s Add bot, total: %i of %i"), *FString(GetName()), PatrolPath.CurrentNumOfPatrollingBots,
			PatrolPath.MaxNumOfPatrollingBots)
		if (PatrolPath.CurrentNumOfPatrollingBots == PatrolPath.MaxNumOfPatrollingBots)
		{
			SetBCanBeTaken(false);
		}
		return true;
	}
	return false;
}

void APatrolPathActor::DeletePatrollingBot()
{
	if (PatrolPath.CurrentNumOfPatrollingBots > 0)
	{
		PatrolPath.CurrentNumOfPatrollingBots--;
	}
	if (PatrolPath.CurrentNumOfPatrollingBots < PatrolPath.MaxNumOfPatrollingBots)
	{
		SetBCanBeTaken(true);
	}
	UE_LOG(LogPRPatrolPath, Log, TEXT("%s Deleted bot, total: %i of %i"), *FString(GetName()), PatrolPath.CurrentNumOfPatrollingBots,
		PatrolPath.MaxNumOfPatrollingBots)
}

// Called when the game starts or when spawned
void APatrolPathActor::BeginPlay()
{
	Super::BeginPlay();
	TArray<USceneComponent*> PatrolPoints;
	RootComponent->GetChildrenComponents(true, PatrolPoints);
	PatrolPath.PatrolPointsAmount = PatrolPoints.Num();
	for (const auto PatrolPoint : PatrolPoints)
	{
		const auto BoxComp = Cast<UBoxComponent>(PatrolPoint);
		if (BoxComp)
		{
			PatrolPath.PatrolPointsPos.Add(TTuple<UBoxComponent*, FVector>(BoxComp, PatrolPoint->GetComponentLocation()));
		}
	}
	if (PatrolPath.PatrolPointsAmount < 5)
	{
		PatrolPath.MaxNumOfPatrollingBots = 1;
	}
	else
	{
		PatrolPath.MaxNumOfPatrollingBots = PatrolPath.PatrolPointsAmount / 5;
	}
}

// Called every frame
void APatrolPathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TTuple<UBoxComponent*, FVector> APatrolPathActor::GetClosestPoint(FVector BotPos) const
{
	UE_LOG(LogPRPatrolPath, Log, TEXT("Started patrolling"))
	float MinDist = FVector::Dist(BotPos, PatrolPath.PatrolPointsPos[0].Value);
	UBoxComponent* BestBoxComp = PatrolPath.PatrolPointsPos[0].Key;
	FVector BetsBoxCompPos = BestBoxComp->GetComponentLocation();
	for (int i = 1; i < PatrolPath.PatrolPointsAmount; i++)
	{
		const float CurrentDist = FVector::Dist(BotPos, PatrolPath.PatrolPointsPos[i].Value);
		UBoxComponent* CurrentBoxComp = PatrolPath.PatrolPointsPos[i].Key;
		const FVector CurrentBoxCompPos = CurrentBoxComp->GetComponentLocation();
		if (MinDist < CurrentDist)
		{
			MinDist = CurrentDist;
			BestBoxComp = CurrentBoxComp;
			BetsBoxCompPos = CurrentBoxCompPos;
		}
	}
	return TTuple<UBoxComponent*, FVector>(BestBoxComp, BetsBoxCompPos);
}

TTuple<UBoxComponent*, FVector> APatrolPathActor::GetNextPatrolPoint(UBoxComponent* CurrentPointRef, FVector CurrentPointPos)
{
	const TTuple<UBoxComponent*, FVector> CurrentPoint = TTuple<UBoxComponent*, FVector>(CurrentPointRef, CurrentPointPos);
	const int CurrentPointInd = PatrolPath.PatrolPointsPos.Find(CurrentPoint);
	int NextPointInd;
	
	UBoxComponent* NextPointRef;
	FVector NextPointPos;
	TTuple<UBoxComponent*, FVector> NextPoint = TTuple<UBoxComponent*, FVector>(NextPointRef, NextPointPos);
	if (CurrentPointInd == PatrolPath.PatrolPointsAmount-1)
	{
		if (GetBIsLooped())
		{
			NextPointInd = 0;
		}
		else
		{
			SetBIsReversed(true);
			NextPointInd = CurrentPointInd - 1;
		}
	}
	else if (0 < CurrentPointInd && CurrentPointInd < PatrolPath.PatrolPointsAmount-1)
	{
		if (GetBIsReversed())
		{
			NextPointInd = CurrentPointInd - 1;
		}
		else
		{
			NextPointInd = CurrentPointInd + 1;
		}
	}
	else
	{
		SetBIsReversed(false);
		NextPointInd = 1;
	}
	NextPointRef = PatrolPath.PatrolPointsPos[NextPointInd].Key;
	NextPointPos = PatrolPath.PatrolPointsPos[NextPointInd].Value;
	NextPoint = TTuple<UBoxComponent*, FVector>(NextPointRef, NextPointPos);
	UE_LOG(LogPRPatrolPath, Warning, TEXT("%s %0.2f %0.2f"), *FString(NextPointRef->GetName()), NextPointPos.X, NextPointPos.Y);
	return NextPoint;
}
