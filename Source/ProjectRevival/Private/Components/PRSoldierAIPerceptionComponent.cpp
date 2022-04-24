// Project Revival. All Rights Reserved


#include "Components/PRSoldierAIPerceptionComponent.h"
#include "PRUtils.h"
#include "HealthComponent.h"
#include "AI/Soldier/SoldierAIController.h"
#include "Player/PlayerCharacter.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "GameFeature/CoverObject.h"
#include "GameFeature/PatrolPathActor.h"
#include "Interfaces/IChangingWorldActor.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"

DEFINE_LOG_CATEGORY(LogPRAIPerception);

FPlayerPositionData UPRSoldierAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
	FPlayerPositionData PlayerPos;
	if (PerceiveActors.Num() == 0)
	{
		//UE_LOG(LogPRAIPerception, Log, TEXT("Enemy: Empty Sight"))
		GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), PerceiveActors);
	}
	if (PerceiveActors.Num() == 0)
	{
		//UE_LOG(LogPRAIPerception, Log, TEXT("Enemy: Empty Hearing"))
		return PlayerPos;
	}
	//UE_LOG(LogPRAIPerception, Log, TEXT("Enemy, Not empty"))

	const auto Controller = Cast<ASoldierAIController>(GetOwner());
	if (!Controller) return PlayerPos;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return PlayerPos;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;

	for (const auto Actor : PerceiveActors)
	{
		const auto HealthComponent = PRUtils::GetCharacterComponent<UHealthComponent>(Actor);
		const auto PerceivePawn = Cast<APawn>(Actor);
		const auto AreEnemies = PerceivePawn && PRUtils::AreEnemies(Controller, PerceivePawn->Controller);
		if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
		{
			const auto CurrentDistance = (Actor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = Actor;
			}
		}
	}
	PlayerPos.SetActor(BestPawn);
	PlayerPos.SetCurrentTime();
	return PlayerPos;
}

bool UPRSoldierAIPerceptionComponent::GetBestCoverWing(EWing Wing, FVector& CoverPos, AActor*& CoverRef)
{
	TArray<AActor*> PerceivedActors;
	AActor* BestCoverRef = nullptr;
	FVector BestCoverPos = FVector(0, 0, 0);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Cover")), PerceivedActors);
	//GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (PerceivedActors.Num() == 0)
	{
		//UE_LOG(LogPRAIPerception, Log, TEXT("Matching Cover objects not found"))
		return false;
	}
	//UE_LOG(LogPRAIPerception, Log, TEXT("Matching Cover objects found"))

	const auto Controller = Cast<ASoldierAIController>(GetOwner());
	if (!Controller) return false;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return false;

	const auto PlayerPos = Controller->GetPlayerPos().GetActor() != nullptr
		                       ? Controller->GetPlayerPos().GetActor()->GetActorLocation()
		                       : FVector::ZeroVector;
	//UE_LOG(LogPRAIPerception, Log, TEXT("CoverPos  input is %s"), *CoverPos.ToString())
	//UE_LOG(LogPRAIPerception, Log, TEXT("PlayerPos input is %s"), *PlayerPos.ToString())
	float BestDist = MAX_FLT;
	FVector StartingCoverPos = CoverPos;
	FVector CoverPosTemp = CoverPos;

	for (const auto Actor : PerceivedActors)
	{
		//UE_LOG(LogPRAIPerception, Log, TEXT("Bool : %s"), Actor->ActorHasTag(TEXT("Cover")) ? TEXT("t") : TEXT("f"))
		if (Actor && Actor->ActorHasTag(TEXT("Cover")))
		{
			//UE_LOG(LogPRAIPerception, Log, TEXT("Actor Has Cover Tag"))
			const auto Cover = Cast<IIChangingWorldActor>(Actor);
			if (Cover && Cover->TryToFindCoverPoint(PlayerPos, CoverPosTemp))
			{
				if (Wing == EWing::Left)
				{
					//UE_LOG(LogPRAIPerception, Log, TEXT("Entered best dist v1"))
					if (FVector::Dist(PlayerPos, CoverPosTemp) > 600.f && BestDist > FVector::Dist(PlayerPos, CoverPosTemp))
					{
						//UE_LOG(LogPRAIPerception, Log, TEXT("Set best dist v1"))
						BestDist = FVector::Dist(PlayerPos, CoverPosTemp);
						BestCoverRef = Actor;
						//UE_LOG(LogPRAIPerception, Log, TEXT("Preset CoverPosTemp X: %0.2f, Y: %0.2f"), CoverPosTemp.X, CoverPosTemp.Y)
						BestCoverPos = CoverPosTemp;
						//UE_LOG(LogPRAIPerception, Log, TEXT("New    BestCoverPos X: %0.2f, Y: %0.2f"), BestCoverPos.X, BestCoverPos.Y)
					}
				}
				else if (Wing == EWing::Center)
				{
					//UE_LOG(LogPRAIPerception, Log, TEXT("Entered best dist v2"))
					if (FVector::Dist(PlayerPos, CoverPosTemp) > 300.f && BestDist > FVector::Dist(PlayerPos, CoverPosTemp))
					{
						//UE_LOG(LogPRAIPerception, Log, TEXT("Set best dist v2"))
						BestDist = FVector::Dist(PlayerPos, CoverPosTemp);
						BestCoverRef = Actor;
						//UE_LOG(LogPRAIPerception, Log, TEXT("Preset CoverPosTemp X: %0.2f, Y: %0.2f"), CoverPosTemp.X, CoverPosTemp.Y)
						BestCoverPos = CoverPosTemp;
						//UE_LOG(LogPRAIPerception, Log, TEXT("New    BestCoverPos X: %0.2f, Y: %0.2f"), BestCoverPos.X, BestCoverPos.Y)
					}
				}
				else if (Wing == EWing::Right)
				{
					//UE_LOG(LogPRAIPerception, Log, TEXT("Entered best dist v3"))
					if (FVector::Dist(PlayerPos, CoverPosTemp) > 600.f && BestDist > FVector::Dist(PlayerPos, CoverPosTemp))
					{
						//UE_LOG(LogPRAIPerception, Log, TEXT("Set best dist v3"))
						BestDist = FVector::Dist(PlayerPos, CoverPosTemp);
						BestCoverRef = Actor;
						//UE_LOG(LogPRAIPerception, Log, TEXT("Preset CoverPosTemp X: %0.2f, Y: %0.2f"), CoverPosTemp.X, CoverPosTemp.Y)
						BestCoverPos = CoverPosTemp;
						//UE_LOG(LogPRAIPerception, Log, TEXT("New    BestCoverPos X: %0.2f, Y: %0.2f"), BestCoverPos.X, BestCoverPos.Y)
					}
				}
			}
		}
	}
	//UE_LOG(LogPRAIPerception, Log, TEXT("*before ending* StartingCoverPos X: %0.2f, Y: %0.2f"), StartingCoverPos.X, StartingCoverPos.Y)
	//UE_LOG(LogPRAIPerception, Log, TEXT("*before ending* BestCoverPos     X: %0.2f, Y: %0.2f"), BestCoverPos.X, BestCoverPos.Y)
	if (StartingCoverPos == BestCoverPos || BestCoverPos.IsZero())
	{
		//UE_LOG(LogPRAIPerception, Log, TEXT("GetBestCoverWing v1 not found"))
		return false;
	}
	else
	{
		//UE_LOG(LogPRAIPerception, Log, TEXT("GetBestCoverWing v2 was found"))
		CoverRef = BestCoverRef;
		CoverPos = BestCoverPos;
		Cast<IIChangingWorldActor>(BestCoverRef)->SetLastCoverPointStatus(false);
		return true;
	}
}

bool UPRSoldierAIPerceptionComponent::GetBestPatrollingPath(FVector& PathPointPos, AActor*& PatrolPathRef)
{
	TArray<AActor*> PerceivedActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPathActor::StaticClass(), PerceivedActors);
	
	if (PerceivedActors.Num() == 0) return false;

	const auto Controller = Cast<ASoldierAIController>(GetOwner());
	if (!Controller) return false;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return false;

	TMap<AActor*, float> ListOfPatrolPaths;
	for (const auto Elem : PerceivedActors)
	{
		const float Dist = FVector::Dist(Pawn->GetActorLocation(), Elem->GetActorLocation());
		ListOfPatrolPaths.Add(Elem, Dist);
	}
	ListOfPatrolPaths.ValueSort(FSortAscending());
	AActor* BestPath = nullptr;
	int BestPathInd = -1;
	int i = 0;
	for (const auto Elem : ListOfPatrolPaths)
	{
		const auto Path = Cast<APatrolPathActor>(Elem.Key);
		UE_LOG(LogPRAIPerception, Warning, TEXT("%s %s"), *FString(Path->GetName()), Path->GetBCanBeTaken()?TEXT("true"):TEXT("false"))
		if (Path && Path->GetBCanBeTaken())
		{
			BestPathInd = i;
			BestPath = Elem.Key;
			break;
		}
		i++;
	}
	if (BestPathInd >= 0)
	{
		PatrolPathRef = BestPath;
		PathPointPos = PatrolPathRef->GetActorLocation();
		return true;
	}
	return false;
}
