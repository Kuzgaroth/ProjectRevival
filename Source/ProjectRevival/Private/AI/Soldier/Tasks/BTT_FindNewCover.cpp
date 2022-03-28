// Project Revival. All Rights Reserved


#include "AI/Soldier/Tasks/BTT_FindNewCover.h"

#include "PlayerCharacter.h"
#include "PRAIPerceptionComponent.h"
#include "AI/Soldier/SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Interfaces/IChangingWorldActor.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectRevival/Public/CoreTypes.h"

UBTT_FindNewCover::UBTT_FindNewCover()
{
	NodeName = "Find New Cover";
}

EBTNodeResult::Type UBTT_FindNewCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return EBTNodeResult::Failed;

	
	
	//Controller->FindNewCover();
	//UE_LOG(LogPRAIDecorators, Log, TEXT("Cover is in search"));
	//return EBTNodeResult::Succeeded;
	TArray<AActor*> PerceivedActors;
	FVector BestCoverPos;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Cover")), PerceivedActors);
	//GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (PerceivedActors.Num()==0)
	{
		UE_LOG(LogPRAIPerception, Log, TEXT("NO COVER FOUND"))
		return EBTNodeResult::Failed;
	}
	//UE_LOG(LogPRAIPerception, Log, TEXT("Cover: Not empty"))
	const auto Blackboard=OwnerComp.GetBlackboardComponent();
	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	const auto PawnPos = Pawn->GetActorLocation();
	const auto PlayerPos=UGameplayStatics::GetActorOfClass(GetWorld(),APlayerCharacter::StaticClass())->GetActorLocation();
	float BestDist = MAX_FLT;
	for (const auto Actor : PerceivedActors)
	{
		//UE_LOG(LogPRAIPerception, Log, TEXT("Bool : %s"), Actor->ActorHasTag(TEXT("Cover")) ? TEXT("t") : TEXT("f"));
		if (Actor && Actor->ActorHasTag(TEXT("Cover")))
		{
			float A = PlayerPos.Y - PawnPos.Y;
			float B = PlayerPos.X - PawnPos.X;
			float C = PlayerPos.Y * B - PawnPos.X * A;
			UE_LOG(LogPRAIPerception, Log, TEXT("%s"),*Actor->GetName());
			FVector CovPos;
			auto Coverable=Cast<IIChangingWorldActor>(Actor);
			if(Coverable)
			{
				UE_LOG(LogPRAIPerception, Log, TEXT("Working"))
				if(Coverable->TryToFindCoverPoint(PlayerPos,CovPos))
				{
					
					UE_LOG(LogPRAIPerception, Log, TEXT("Found cover"))
					Blackboard->SetValueAsVector("CoverPos",CovPos);
					return EBTNodeResult::Succeeded;
				}
			}
			//const auto CoverPos = Actor->GetActorLocation();
			//UE_LOG(LogPRAIPerception, Log, TEXT("Cover pos X: %0.2f, Y: %0.2f"), CoverPos.X, CoverPos.Y)
			//float LineEquation = A * CoverPos.X + B * CoverPos.Y + C;
			//float DistToLine = abs(A * CoverPos.X + B * CoverPos.Y + C) / sqrt(A * A + B * B);
			//UE_LOG(LogPRAIPerception, Log, TEXT("Dist to Line: %0.2f"), DistToLine)
			//UE_LOG(LogPRAIPerception, Log, TEXT("Line Equation: %0.2f"), LineEquation)
			//BestCoverPos=CoverPos;
			//if (Wing == EWing::Left)
			//{
			//	if (LineEquation > 0.0f && DistToLine > 300.0f && BestDist > FVector::Dist(PawnPos, CoverPos))
			//	{
			//		BestDist = FVector::Dist(PawnPos, CoverPos);
			//		BestCoverPos = CoverPos;
			//	}
			//}
			//else if (Wing == EWing::Center && BestDist > FVector::Dist(PawnPos, CoverPos))
			//{
			//	if (DistToLine <= 300.0f)
			//	{
			//		BestDist = FVector::Dist(PawnPos, CoverPos);
			//		BestCoverPos = CoverPos;
			//	}
			//}
			//else if (Wing == EWing::Right && BestDist > FVector::Dist(PawnPos, CoverPos))
			//{
			//	if (LineEquation < 0.0f && DistToLine > 300.0f && BestDist > FVector::Dist(PawnPos, CoverPos))
			//	{
			//		BestDist = FVector::Dist(PawnPos, CoverPos);
			//		BestCoverPos = CoverPos;
			//	}
			//}

		}
	}
	return EBTNodeResult::Failed;
	
}
