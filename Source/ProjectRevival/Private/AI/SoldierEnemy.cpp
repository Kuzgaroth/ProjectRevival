// Project Revival. All Rights Reserved

#define COVER_TRACE_CHANNEL ECC_GameTraceChannel3


#include "AI/SoldierEnemy.h"
#include "BaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIWeaponComponent.h"
#include "BrainComponent.h"
#include "HealthBarWidget.h"
#include "HealthComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Soldier/SoldierAIController.h"

ASoldierEnemy::ASoldierEnemy(const FObjectInitializer& ObjectInitializer) :Super(
	ObjectInitializer.SetDefaultSubobjectClass<UAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ABaseAIController::StaticClass();
	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	HealthWidgetComponent->SetupAttachment(RootComponent);
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);

	SideMoveAmount = 0;
	
	HState = HealthStateSoldier::FULL_HEALTH;
}

void ASoldierEnemy::OnDeath()
{
	Super::OnDeath();

	const auto PRController = Cast<ASoldierAIController>(GetController());
	if (PRController && PRController->BrainComponent)
	{
		PRController->BrainComponent->Cleanup();
	}
}

void ASoldierEnemy::Tick(float DeltaSeconds)
{
	UpdateHealthWidgetVisibility();
}

void ASoldierEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AICon = Cast<ASoldierAIController>(GetController());
	UE_LOG(LogTemp, Log, TEXT("After AICon cast"));
	if (AICon)
	{
		BBComp = AICon->GetBlackboardComponent();
		UE_LOG(LogTemp, Log, TEXT("Check if cast was successful"));
		Cast<ASoldierAIController>(GetController())->StartEnteringCoverDelegate.AddDynamic(this, &ASoldierEnemy::StartCoverSoldier);
		Cast<ASoldierAIController>(GetController())->StartExitingCoverDelegate.AddDynamic(this, &ASoldierEnemy::StopCoverSoldier);
		Cast<ASoldierAIController>(GetController())->StartCoverSideMovingDelegate.AddDynamic(this, &ASoldierEnemy::ChangeCoverSide);
	}
	UpdateHStateBlackboardKey(3);
	UE_LOG(LogTemp, Log, TEXT("SPECIAL"));
}

void ASoldierEnemy::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);
	CoverData.IsTurning = false;
	CoverData.IsInCoverTransition = false;
	CoverData.IsInFireTransition = false;
	CoverData.IsSwitchingCoverType = false;
	CoverData.IsFiring = false;
}

void ASoldierEnemy::OnHealthChanged(float CurrentHealth, float HealthDelta)
{
	Super::OnHealthChanged(CurrentHealth, HealthDelta);

	if (HealthComponent->GetHealthPercentage() > 0.75)
	{
		HState = HealthStateSoldier::FULL_HEALTH;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(3);
	}
	else if ((HealthComponent->GetHealthPercentage() <= 0.75) && (HealthComponent->GetHealthPercentage() > 0.5))
	{
		HState = HealthStateSoldier::SLIGHTLY_INJURED;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(2);
	}
	else if ((HealthComponent->GetHealthPercentage() <= 0.5) && (HealthComponent->GetHealthPercentage() > 0.25))
	{
		HState = HealthStateSoldier::BADLY_INJURED;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(1);
	}
	else
	{
		HState = HealthStateSoldier::NEAR_DEATH;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(0);
	}
	
	const auto HealthBarWidget = Cast<UHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());

	if (!HealthBarWidget) return;

	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercentage());
}

void ASoldierEnemy::UpdateHStateBlackboardKey(uint8 EnumKey)
{
	if(BBComp)
	{
		BBComp->SetValueAsEnum("HState", EnumKey);
		UE_LOG(LogTemp, Log, TEXT("BBComp was updated %i"), EnumKey);
	}
}

void ASoldierEnemy::UpdateHealthWidgetVisibility()
{
	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());

	HealthWidgetComponent->SetVisibility(Distance<HealthVisibilityDistance, true);
}

void ASoldierEnemy::StartCoverSoldier(const FVector& CoverPos)
{
	TArray<FHitResult> Hits;
	const bool TraceResult = UKismetSystemLibrary::LineTraceMulti(GetWorld(), GetActorLocation(),
		FVector(CoverPos.X, CoverPos.Y, CoverPos.Z + 1.0), UEngineTypes::ConvertToTraceType(COVER_TRACE_CHANNEL),
		false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, Hits, true);
	if (!TraceResult)
	{
		StopEnteringCoverDelegate.Broadcast();
		return;
	}
	
	FHitResult CoverHit = Hits.Last();
	CoverData.CoverObject = CoverHit.GetActor();
	if (!CoverData.CoverObject)
	{
		StopEnteringCoverDelegate.Broadcast();
		return;
	}
	
	const auto AISoldierController = Cast<ASoldierAIController>(GetController());
	if (!AISoldierController)
	{
		StopEnteringCoverDelegate.Broadcast();
		return;
	}
	
	WeaponComponent->StopFire();
	AISoldierController->MoveToActor(CoverData.CoverObject);

	CoverData.CoverObject->ActorHasTag(FName(TEXT("High"))) ? CoverData.CoverType = High : CoverData.CoverType = Low;
	CoverData.CoverSide = CheckSideByNormal(GetActorForwardVector(), CoverHit.Normal);
	CoverData.CoverPart = GetCoverPart(0);
	CoverData.IsInCoverTransition = true;
	
	const bool Sup = Super::StartCover_Internal(CoverHit);
	if (!Sup)
	{
		StopEnteringCoverDelegate.Broadcast();
		return;
	}
	bUseControllerRotationYaw = false; //We can delete this line cause this parameter is already set permanently in constructor
}

void ASoldierEnemy::StartCoverSoldierFinish()
{
	CoverData.IsInCoverTransition = false;
	StopEnteringCoverDelegate.Broadcast();
}

void ASoldierEnemy::StopCoverSoldier()
{
	const bool Sup = Super::StopCover_Internal();
	if (!Sup)
	{
		StopExitingCoverDelegate.Broadcast();
		return;
	}
	CoverData.StopCover();
	CoverData.CoverType = ECoverType::None;
	CoverData.CoverSide = ECoverSide::CSNone;
	CoverData.CoverPart = ECoverPart::CPNone;
	bUseControllerRotationYaw = false;
}

void ASoldierEnemy::StopCoverSoldierFinish()
{
	CoverData.IsInCoverTransition = false;
	StopExitingCoverDelegate.Broadcast();
}

void ASoldierEnemy::ChangeCoverSide(const float Amount)
{
	if (GetController() && (Amount != 0.0f) )
	{
		if (CoverData.IsInTransition()) {
			StopCoverSideMovingDelegate.Broadcast();
			return;
		}
		if (CoverData.IsInCover())
		{
			CoverData.TurnStart(Amount);
			if (!CoverData.IsTurning)
			{
				StopCoverSideMovingDelegate.Broadcast();
				/*If CoverData.IsTurning == true and we've started "Turn" animation then we will execute following lines
				triggering function "ChangeCoverSideAfterAnim" from AnimBP*/
				const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
				const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				this->AddMovementInput(Direction, Amount);
				
			}
		}
	}
}

void ASoldierEnemy::ChangeCoverSideFinish()
{
	CoverData.IsTurning = false;
	StopCoverSideMovingDelegate.Broadcast();
	const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	this->AddMovementInput(Direction, SideMoveAmount);
}

ECoverType ASoldierEnemy::CheckCover()
{
	FHitResult HitResult;
	return Super::CoverTrace(HitResult);
}

TEnumAsByte<ECoverSide> ASoldierEnemy::CheckSideByNormal(FVector Forward, FVector Normal)
{
	Forward.Normalize();
	Normal = - Normal;
	float CosNormal = Normal.CosineAngle2D(FVector(0, 1, 0));
	float CosForward = Forward.CosineAngle2D(FVector(0, 1, 0));
	if (CosNormal >= CosForward) return Left;
	else return Right;
}

TEnumAsByte<ECoverPart> ASoldierEnemy::GetCoverPart(int8 PartPos)
{
	return PartPos == 0 ? Middle : Edge;
}

void ASoldierEnemy::CoverCrouch()
{
	CoverData.TrySwitchCoverType(this);
}

FCoverData& ASoldierEnemy::GetCoverData()
{
	return CoverData;
}

bool ASoldierEnemy::IsInCover()
{
	return CoverData.IsInCover() && !CoverData.IsInTransition();
}