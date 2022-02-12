// Project Revival. All Rights Reserved

#define COVER_TRACE_CHANNEL ECC_GameTraceChannel3


#include "AI/SoldierEnemy.h"
#include "AICharacter.h"
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

void ASoldierEnemy::BeginPlay()
{
	Super::BeginPlay();
	SideMoveAmount = 0;
	CoverData.IsTurning = false;
	CoverData.IsInCoverTransition = false;
	CoverData.IsInFireTransition = false;
	CoverData.IsSwitchingCoverType = false;
	CoverData.IsFiring = false;
	bIsInCoverBP = false;
}

// void ASoldierEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
// 	PlayerInputComponent->BindAxis("MoveForward",this,&ASoldierEnemy::MoveForward);
// 	PlayerInputComponent->BindAxis("MoveRight",this,&ASoldierEnemy::MoveRight);
// 	PlayerInputComponent->BindAxis("LookUp",this,&ASoldierEnemy::AddControllerPitchInput);
// 	PlayerInputComponent->BindAxis("TurnAround",this,&ASoldierEnemy::AddControllerYawInput);
// 	PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed,this, &ABaseCharacter::Jump);
// 	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Pressed,this, &ASoldierEnemy::StartRun);
// 	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Released,this, &ASoldierEnemy::StopRun);
// 	PlayerInputComponent->BindAction("Fire",EInputEvent::IE_Pressed,WeaponComponent, &UWeaponComponent::StartFire);
// 	PlayerInputComponent->BindAction("Fire",EInputEvent::IE_Released,WeaponComponent, &UWeaponComponent::StopFire);
// 	
// 	AbilitySystemComponent->BindAbilityActivationToInputComponent(this->InputComponent,
// 	FGameplayAbilityInputBinds(FString("ConfirmTarget"),
// 	FString("CancelTarget"), FString("EGASInputActions")));
// }
//
// void ASoldierEnemy::MoveForward(float Amount)
// {
// 	IsMovingForward = Amount>0;
// 	if ( (this->GetController() != nullptr) && (Amount != 0.0f))
// 	{
// 		const FRotator Rotation = this->GetController()->GetControlRotation();
// 		const FRotator YawRotation(0, Rotation.Yaw, 0);
// 		
// 		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
// 		
// 		this->AddMovementInput(Direction, Amount);
// 	}
// }
//
// void ASoldierEnemy::MoveRight(float Amount)
// {
// 	if ( (this->GetController() != nullptr) && (Amount != 0.0f))
// 	{
// 		const FRotator Rotation = this->GetController()->GetControlRotation();
// 		const FRotator YawRotation(0, Rotation.Yaw, 0);
// 		
// 		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
// 		
// 		this->AddMovementInput(Direction, Amount);
// 	}
// }
//
// void ASoldierEnemy::StartRun()
// {
// 	bWantsToRun=true;
// }
//
// void ASoldierEnemy::StopRun()
// {
// 	bWantsToRun=false;
// }
//
// bool ASoldierEnemy::IsRunning() const
// {
// 	return bWantsToRun && IsMovingForward && !GetVelocity().IsZero();
// }

void ASoldierEnemy::OnDeath()
{
	ABaseCharacter::OnDeath();

	const auto PRController = Cast<ASoldierAIController>(GetController());
	if (PRController && PRController->BrainComponent)
	{
		PRController->BrainComponent->Cleanup();
	}
}

void ASoldierEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASoldierEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	Cast<ASoldierAIController>(GetController())->StartEnteringCoverDelegate.AddDynamic(this, &ASoldierEnemy::StartCoverSoldier);
	Cast<ASoldierAIController>(GetController())->StartExitingCoverDelegate.AddDynamic(this, &ASoldierEnemy::StopCoverSoldier);
	Cast<ASoldierAIController>(GetController())->StartCoverSideMovingDelegate.AddDynamic(this, &ASoldierEnemy::ChangeCoverSide);
}

void ASoldierEnemy::OnHealthChanged(float CurrentHealth, float HealthDelta)
{
	Super::OnHealthChanged(CurrentHealth, HealthDelta);
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
		return;
	}
	
	FHitResult CoverHit = Hits.Last();
	CoverData.CoverObject = CoverHit.GetActor();
	if (!CoverData.CoverObject)
	{
		return;
	}
	
	const auto AISoldierController = Cast<ASoldierAIController>(GetController());
	if (!AISoldierController)
	{
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
		return;
	}
	bUseControllerRotationYaw = false; //We can delete this line cause this parameter is already set permanently in constructor
	StartEnteringCoverForAnimDelegate.Broadcast();
}

void ASoldierEnemy::StartCoverSoldierFinish()
{
	CoverData.IsInCoverTransition = false;
	bIsInCoverBP = true;
	StopEnteringCoverDelegate.Broadcast();
}

void ASoldierEnemy::StopCoverSoldier()
{
	const bool Sup = Super::StopCover_Internal();
	if (!Sup)
	{
		return;
	}
	CoverData.StopCover();
	bUseControllerRotationYaw = false;
	CoverData.IsInCoverTransition = true;
	StartExitingCoverForAnimDelegate.Broadcast();
}

void ASoldierEnemy::StopCoverSoldierFinish()
{
	CoverData.IsInCoverTransition = false;
	CoverData.CoverType = ECoverType::None;
	CoverData.CoverSide = ECoverSide::CSNone;
	CoverData.CoverPart = ECoverPart::CPNone;
	bIsInCoverBP = false;
	StopExitingCoverDelegate.Broadcast();
}

void ASoldierEnemy::ChangeCoverSide(const float Amount)
{
	if (GetController())
	{
		if (CoverData.IsInTransition()) {
			return;
		}
		if (CoverData.IsInCover())
		{
			CoverData.TurnStart(Amount);
			if (CoverData.IsTurning)
			{
				SideMoveAmount = Amount;
				StartCoverSideMovingForAnimDelegate.Broadcast();
			}
		}
	}
}

void ASoldierEnemy::ChangeCoverSideFinish()
{
	CoverData.IsTurning = false;
	switch (CoverData.CoverSide)
	{
		case Left:
			CoverData.CoverSide = Right;
			break;
		case Right:
			CoverData.CoverSide = Left;
			break;
		default:
			CoverData.CoverSide = CSNone;
	}
	const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	this->AddMovementInput(Direction, SideMoveAmount);
	StopCoverSideMovingDelegate.Broadcast();
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

// bool ASoldierEnemy::IsInCover()
// {
// 	return CoverData.IsInCover() && !CoverData.IsInTransition();
// }