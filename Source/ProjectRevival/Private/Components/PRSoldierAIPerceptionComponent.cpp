// Project Revival. All Rights Reserved


#include "Components/PRSoldierAIPerceptionComponent.h"
#include "PRUtils.h"
#include "HealthComponent.h"
#include "AI/Soldier/SoldierAIController.h"
#include "Player/PlayerCharacter.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "GameFeature/CoverObject.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"

DEFINE_LOG_CATEGORY(LogPRAIPerception);

AActor* UPRSoldierAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
	if (PerceiveActors.Num()==0)
	{
		//UE_LOG(LogPRAIPerception, Log, TEXT("Enemy: Empty Sight"))
		GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), PerceiveActors);
	}
	if (PerceiveActors.Num()==0)
	{
		//UE_LOG(LogPRAIPerception, Log, TEXT("Enemy: Empty Hearing"))
		return nullptr;
	}
	//UE_LOG(LogPRAIPerception, Log, TEXT("Enemy, Not empty"))

	const auto Controller = Cast<ASoldierAIController>(GetOwner());
	if (!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

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
	return BestPawn;
}

FVector UPRSoldierAIPerceptionComponent::GetBestCoverWing(EWing Wing)
{
	TArray<AActor*> PerceivedActors;
	FVector BestCoverPos;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Cover")), PerceivedActors);
	//GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (PerceivedActors.Num()==0)
	{
		UE_LOG(LogPRAIPerception, Log, TEXT("NO COVER FOUND"))
		return FVector::ZeroVector;
	}
	//UE_LOG(LogPRAIPerception, Log, TEXT("Cover: Not empty"))

	const auto Controller = Cast<ASoldierAIController>(GetOwner());
	if (!Controller) return FVector::ZeroVector;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return FVector::ZeroVector;

	const auto PawnPos = Pawn->GetActorLocation();
	const auto PlayerPos = Controller->GetPlayerPos();
	float BestDist = MAX_FLT;
	
	for (const auto Actor : PerceivedActors)
	{
		//UE_LOG(LogPRAIPerception, Log, TEXT("Bool : %s"), Actor->ActorHasTag(TEXT("Cover")) ? TEXT("t") : TEXT("f"));
		if (Actor && Actor->ActorHasTag(TEXT("Cover")))
		{

			float A = PlayerPos.Y - PawnPos.Y;
			float B = PlayerPos.X - PawnPos.X;
			float C = PlayerPos.Y * B - PawnPos.X * A;
			const auto CoverPos = Actor->GetActorLocation();
			UE_LOG(LogPRAIPerception, Log, TEXT("Cover pos X: %0.2f, Y: %0.2f"), CoverPos.X, CoverPos.Y)
			float LineEquation = A * CoverPos.X + B * CoverPos.Y + C;
			float DistToLine = abs(A * CoverPos.X + B * CoverPos.Y + C) / sqrt(A * A + B * B);
			UE_LOG(LogPRAIPerception, Log, TEXT("Dist to Line: %0.2f"), DistToLine)
			UE_LOG(LogPRAIPerception, Log, TEXT("Line Equation: %0.2f"), LineEquation)
			BestCoverPos=CoverPos;
			if (Wing == EWing::Left)
			{
				if (LineEquation > 0.0f && DistToLine > 300.0f && BestDist > FVector::Dist(PawnPos, CoverPos))
				{
					BestDist = FVector::Dist(PawnPos, CoverPos);
					BestCoverPos = CoverPos;
				}
			}
			else if (Wing == EWing::Center && BestDist > FVector::Dist(PawnPos, CoverPos))
			{
				if (DistToLine <= 300.0f)
				{
					BestDist = FVector::Dist(PawnPos, CoverPos);
					BestCoverPos = CoverPos;
				}
			}
			else if (Wing == EWing::Right && BestDist > FVector::Dist(PawnPos, CoverPos))
			{
				if (LineEquation < 0.0f && DistToLine > 300.0f && BestDist > FVector::Dist(PawnPos, CoverPos))
				{
					BestDist = FVector::Dist(PawnPos, CoverPos);
					BestCoverPos = CoverPos;
				}
			}

		}
	}
	return BestCoverPos;
}
