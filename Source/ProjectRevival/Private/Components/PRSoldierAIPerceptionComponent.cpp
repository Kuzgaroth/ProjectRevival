// Project Revival. All Rights Reserved


#include "Components/PRSoldierAIPerceptionComponent.h"
#include "PRUtils.h"
#include "HealthComponent.h"
#include "AI/Soldier/SoldierAIController.h"
#include "Player/PlayerCharacter.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "GameFeature/CoverObject.h"
#include "Interfaces/IChangingWorldActor.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"

DEFINE_LOG_CATEGORY(LogPRAIPerception);

FPlayerPositionData UPRSoldierAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
	FPlayerPositionData PlayerPos;
	if (PerceiveActors.Num()==0)
	{
		UE_LOG(LogPRAIPerception, Log, TEXT("Enemy: Empty Sight"))
		GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), PerceiveActors);
	}
	if (PerceiveActors.Num()==0)
	{
		UE_LOG(LogPRAIPerception, Log, TEXT("Enemy: Empty Hearing"))
		return PlayerPos;
	}
	UE_LOG(LogPRAIPerception, Log, TEXT("Enemy, Not empty"))

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
			const auto CurrentDistance = (Actor->GetActorLocation()-Pawn->GetActorLocation()).Size();
			if (CurrentDistance<BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = Actor;
			}
		}
	}
	PlayerPos.SetActor(BestPawn);
	return PlayerPos;
}

bool UPRSoldierAIPerceptionComponent::GetBestCoverWing(EWing Wing, FVector& CoverPos, AActor*& CoverRef)
{
	UE_LOG(LogPRAIPerception, Log, TEXT("Perception: CoverPos  input is %s"), *CoverPos.ToString())
	UE_LOG(LogPRAIPerception, Log, TEXT("Perception: PlayerPos input is %s"), *CoverPos.ToString())
	TArray<AActor*> PerceivedActors;
	AActor* BestCoverRef = nullptr;
	FVector BestCoverPos = FVector(0, 0, 0);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Cover")), PerceivedActors);
	//GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (PerceivedActors.Num()==0)
	{
		UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Cover: matching objects not found"))
		return false;
	}
	UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Cover: matching objects found"))

	const auto Controller = Cast<ASoldierAIController>(GetOwner());
	if (!Controller) return false;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return false;

	const auto PawnPos = Pawn->GetActorLocation();
	const auto PlayerPos = (Controller->GetPlayerPos().GetActor()!=nullptr) ? Controller->GetPlayerPos().GetActor()->GetActorLocation():FVector::ZeroVector;
	float BestDist = MAX_FLT;
	FVector StartingCoverPos = CoverPos;
	FVector CoverPosTemp = CoverPos;
	
	for (const auto Actor : PerceivedActors)
	{
		UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Bool : %s"), Actor->ActorHasTag(TEXT("Cover")) ? TEXT("t") : TEXT("f"));
		if (Actor && Actor->ActorHasTag(TEXT("Cover")))
		{
			UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Actor Has Cover Tag"))
			const auto Cover = Cast<IIChangingWorldActor>(Actor);
			if (Cover && Cover->TryToFindCoverPoint(PlayerPos, CoverPosTemp))
			{
				float A = PlayerPos.Y - PawnPos.Y;
				float B = PlayerPos.X - PawnPos.X;
				float C = PlayerPos.Y * B - PawnPos.X * A;
				UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Cover pos X: %0.2f, Y: %0.2f"), CoverPosTemp.X, CoverPosTemp.Y)
				float LineEquation = A * CoverPosTemp.X + B * CoverPosTemp.Y + C;
				float DistToLine = abs(A * CoverPosTemp.X + B * CoverPosTemp.Y + C) / sqrt(A * A + B * B);
				UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Dist to Line: %0.2f"), DistToLine)
				UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Line Equation: %0.2f"), LineEquation)
				if (Wing == EWing::Left)
				{
					UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Entered best dist v1"))
					if (LineEquation > 0.0f && DistToLine > 400.0f && BestDist > FVector::Dist(PawnPos, CoverPosTemp))
					{
						UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Set best dist v1"))
						BestDist = FVector::Dist(PawnPos, CoverPosTemp);
						BestCoverRef = Actor;
						BestCoverPos = CoverPosTemp;
					}
				}
				else if (Wing == EWing::Center && BestDist > FVector::Dist(PawnPos, CoverPosTemp))
				{
					UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Entered best dist v2"))
					if (DistToLine <= 400.0f)
					{
						UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Set best dist v2"))
						BestDist = FVector::Dist(PawnPos, CoverPosTemp);
						BestCoverRef = Actor;
						BestCoverPos = CoverPosTemp;
					}
				} 
				else if (Wing == EWing::Right && BestDist > FVector::Dist(PawnPos, CoverPosTemp))
				{
					UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Entered best dist v3"))
					if (LineEquation < 0.0f && DistToLine > 400.0f && BestDist > FVector::Dist(PawnPos, CoverPosTemp))
					{
						UE_LOG(LogPRAIPerception, Log, TEXT("Perception: Set best dist v3"))
						BestDist = FVector::Dist(PawnPos, CoverPosTemp);
						BestCoverRef = Actor;
						BestCoverPos = CoverPosTemp;
					}
				}
			}
		}
	}
	UE_LOG(LogPRAIPerception, Log, TEXT("Perception: *before ending* StartingCoverPos X: %0.2f, Y: %0.2f"), StartingCoverPos.X, StartingCoverPos.Y)
	UE_LOG(LogPRAIPerception, Log, TEXT("Perception: *before ending* BestCoverPos     X: %0.2f, Y: %0.2f"), BestCoverPos.X, BestCoverPos.Y)
	if (StartingCoverPos == BestCoverPos || BestCoverPos.IsZero())
	{
		UE_LOG(LogPRAISoldier, Log, TEXT("Perception: GetBestCoverWing v1 not found"))
		return false;
	} else
	{
		UE_LOG(LogPRAISoldier, Log, TEXT("Perception: GetBestCoverWing v2 was found"))
		CoverRef = BestCoverRef;
		CoverPos = BestCoverPos;
		if (CoverRef)
		{
			UE_LOG(LogPRAISoldier, Log, TEXT("Perception: PerceptionCoverRef class is %s"), *CoverRef->GetName());
		} else
		{
			UE_LOG(LogPRAISoldier, Log, TEXT("Perception: SUCK SOME DICK"), *CoverRef->GetName());	
		}
		return true;
	}
}
