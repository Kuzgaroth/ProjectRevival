// Project Revival. All Rights Reserved

#define COVER_TRACE_CHANNEL ECC_GameTraceChannel3


#include "AI/SoldierEnemy.h"
#include "AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIWeaponComponent.h"
#include "BrainComponent.h"
#include "HealthBarWidget.h"
#include "HealthComponent.h"
#include "SoldierRifleWeapon.h"
#include "AbilitySystem/Abilities/GrenadeAbility.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Soldier/SoldierAIController.h"
#include "ProjectRevival/Public/CoreTypes.h"

DEFINE_LOG_CATEGORY(LogPRAISoldier)

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
	bIsFiringBP = false;
}

/*Чтобы протестить бота, нужно раскомментить следующие функции вплоть до IsRunning(),
 *также раскомментить подряд идущие строки ближе к концу .h файла
 *и в блюпринте core/BP_PRGameModeBase поменять параметр Default Pawn Class с BP_PlayerCharacter на BP_SoldierEnemyCharacter*/

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
// 	PlayerInputComponent->BindAction("Cover",EInputEvent::IE_Released,this, &ASoldierEnemy::ThrowGrenadeCaller);
// 	PlayerInputComponent->BindAction("Fire",EInputEvent::IE_Pressed,WeaponComponent, &UWeaponComponent::StartFire);
// 	PlayerInputComponent->BindAction("Fire",EInputEvent::IE_Released,WeaponComponent, &UWeaponComponent::StopFire);
// 	
// 	AbilitySystemComponent->BindAbilityActivationToInputComponent(this->InputComponent,
// 	FGameplayAbilityInputBinds(FString("ConfirmTarget"),
// 	FString("CancelTarget"), FString("EGASInputActions")));
// }

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
	PRController->OnBotDied.Broadcast(PRController);
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
	Cast<ASoldierAIController>(GetController())->PlayerPosDelegate.AddDynamic(this, &ASoldierEnemy::StartFiring);
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

void ASoldierEnemy::StartCoverSoldier(const FVector& CoverPos, AActor* CoverRef)
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverSoldier() was called"));
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bIgnoreTouches = true;
	CollisionParams.AddIgnoredActor(this);
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: CoverPos: %s"), *CoverPos.ToString());
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: CoverRef name is %s"), *CoverRef->GetName());
	CoverData.CoverObject = CoverRef;
	GetWorld()->LineTraceSingleByChannel(HitResult,CoverPos, CoverRef->GetActorLocation(),ECollisionChannel::ECC_Visibility, CollisionParams);
	if (HitResult.bBlockingHit)
	{
		UE_LOG(LogPRAISoldier, Log, TEXT("Character: The blocking hit objest was: %s"), *HitResult.GetActor()->GetName());
		UE_LOG(LogPRAISoldier, Log, TEXT("Character: The blocking hit normal was: %s"), *HitResult.Normal.ToString());
	}
	else
	{
		UE_LOG(LogPRAISoldier, Log, TEXT("Character: CoverOwnerPos turned to be clear"))
		CleanCoverData();
		return;
	}
	
	WeaponComponent->StopFire();

	CoverData.CoverObject = CoverRef;
	CoverData.CoverObject->ActorHasTag(FName(TEXT("High"))) ? CoverData.CoverType = High : CoverData.CoverType = Low;
	CoverData.CoverSide = Right;
	CoverData.CoverPart = GetCoverPart(0);

	ASoldierAIController* Controller = Cast<ASoldierAIController>(GetController());
	if (!Controller)
	{
		CleanCoverData();
		return;
	} //here must be rotation added

	if (!Super::StartCover_Internal(HitResult))
	{
		GetCharacterMovement()->SetPlaneConstraintEnabled(false);
		CleanCoverData();
		return;
	}
	bUseControllerRotationYaw = false;
	CoverData.IsInCoverTransition = true;
	// StartEnteringCoverForAnimDelegate.Broadcast();
}

void ASoldierEnemy::StartCoverSoldierFinish()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverSoldierFinish() was called"));
	CoverData.IsInCoverTransition = false;
	bIsInCoverBP = true;
	StopEnteringCoverDelegate.Broadcast();
}

void ASoldierEnemy::StopCoverSoldier()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StopCoverSoldier() was called"));
	const bool Sup = Super::StopCover_Internal();
	if (!Sup)
	{
		return;
	}
	CoverData.StopCover();
	bUseControllerRotationYaw = false;
	CoverData.IsInCoverTransition = true;
	// StartExitingCoverForAnimDelegate.Broadcast();
}

void ASoldierEnemy::StopCoverSoldierFinish()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StopCoverSoldierFinish() was called"));
	CleanCoverData();
	bIsInCoverBP = false;
	StopExitingCoverDelegate.Broadcast();
}

void ASoldierEnemy::ChangeCoverSide(const float Amount)
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: ChangeCoverSide() was called"));
	if (CoverData.IsInTransition()) {return;}
	if (!CoverData.IsInCover()){return;}
	SideMoveAmount = Amount;
	if (SideMoveAmount == 0)
	{
		CoverData.IsTurning = true;
	}
	else
	{
		CoverData.TurnStart(Amount);
		if (!CoverData.IsTurning)
		{
			ChangeCoverSideFinish();
			// StartCoverSideMovingForAnimDelegate.Broadcast();
		}
	}
}

void ASoldierEnemy::ChangeCoverSideFinish()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: ChangeCoverSideFinish() was called"));
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
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bIgnoreTouches = true;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult,GetActorLocation(),GetActorForwardVector()*RootDelta+GetActorLocation(),ECollisionChannel::ECC_Visibility, CollisionParams);
	if (HitResult.bBlockingHit) 
	{
		CoverData.CoverObject = HitResult.GetActor();
		if (CoverData.CoverObject->ActorHasTag(FName(TEXT("High"))) && CoverData.CoverType == Low) {CoverData.IsSwitchingCoverType = true;}
		if (CoverData.CoverObject->ActorHasTag(FName(TEXT("Low"))) && CoverData.CoverType == High) {CoverData.IsSwitchingCoverType = true;}
		if (!CoverData.IsSwitchingCoverType)
		{
			StopCoverSideMovingDelegate.Broadcast();
		}
	}
}

void ASoldierEnemy::ChangeCoverTypeFinish()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: ChangeCoverTypeFinish() was called"));
	CoverData.IsSwitchingCoverType = false;
	switch (CoverData.CoverType)
	{
	case High:
		CoverData.CoverType = Low;
		break;
	case Low:
		CoverData.CoverType = High;
		break;
	default:
		CoverData.CoverType = None;
	}
	StopCoverSideMovingDelegate.Broadcast();
}

void ASoldierEnemy::StartCoverToFire()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverToFire() was called"));
	if (GetCoverIndex() < 2) {return;}
	if (CoverData.IsInTransition()) {return;}
	if (!CoverData.IsInCover()) {return;}
	CoverData.IsInFireTransition = true;
	// StartCoverToFireForAnimDelegate.Broadcast();
}

void ASoldierEnemy::StartCoverToFireFinish()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverToFireFinish() was called"));
	CoverData.IsInFireTransition = false;
	CoverData.IsFiring = true;
	StartFiring(PlayerCoordinates);
}

void ASoldierEnemy::StartCoverFromFire()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverFromFire() was called"));
	if (CoverData.IsInTransition()) {return;}
	if (!CoverData.IsInCover()) {return;}
	CoverData.IsFiring = false;
	CoverData.IsInFireTransition = true;
	// StartCoverFromFireForAnimDelegate.Broadcast();
}

void ASoldierEnemy::StartCoverFromFireFinish()
{	
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverFromFireFinish() was called"));
	CoverData.IsInFireTransition = false;
	StopFiring();
}

void ASoldierEnemy::StartFiring(const FPlayerPositionData& PlayerPos)
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() was called"));
	PlayerCoordinates = PlayerPos;
	// UE_LOG(LogPRAISoldier, Log, TEXT("Bot started firing at X: %0.1f, Y: %0.1f"), PlayerCoordinates.GetActorPosition().X, PlayerCoordinates.GetActorPosition().Y)
	if (CoverData.IsInCover() && bIsInCoverBP && !CoverData.IsFiring && !bIsFiringBP && GetCoverIndex() >= 2)
	{
		StartCoverToFire();
		return;
	}
	else if (CoverData.IsInCover() && bIsInCoverBP && CoverData.IsFiring && !bIsFiringBP)
	{
		RifleRef = Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon());
		RifleRef->StartFire();
		bIsFiringBP = true;
		StartFireDelegate.Broadcast();
		if (RifleRef)
		{
			RifleRef->StoppedFireInWeaponDelegate.AddDynamic(this, &ASoldierEnemy::StopFiring);
		}
		return;
	}
	else if (!CoverData.IsInCover() && !bIsInCoverBP && !bIsFiringBP)
	{
		RifleRef = Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon());
		RifleRef->StartFire();
		bIsFiringBP = true;
		StartFireDelegate.Broadcast();
		if (RifleRef)
		{
			RifleRef->StoppedFireInWeaponDelegate.AddDynamic(this, &ASoldierEnemy::StopFiring);
		}
		return;
	}
}

void ASoldierEnemy::StopFiring()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StopFiring() was called"));
	if (CoverData.IsInCover() && bIsInCoverBP && CoverData.IsFiring && bIsFiringBP)
	{
		StartCoverFromFire();
		return;
	}
	else if (CoverData.IsInCover() && bIsInCoverBP && !CoverData.IsFiring && bIsFiringBP)
	{
		bIsFiringBP = false;
		if (RifleRef)
		{
			RifleRef->StoppedFireInWeaponDelegate.RemoveDynamic(this, &ASoldierEnemy::StopFiring);
			RifleRef = nullptr;
		}
		StopFireDelegate.Broadcast();
		return;
	}
	else if (!CoverData.IsInCover() && !bIsInCoverBP && bIsFiringBP)
	{
		bIsFiringBP = false;
		if (RifleRef)
		{
			RifleRef->StoppedFireInWeaponDelegate.RemoveDynamic(this, &ASoldierEnemy::StopFiring);
			RifleRef = nullptr;
		}
		StopFireDelegate.Broadcast();
		return;
	}
}

ECoverType ASoldierEnemy::CheckCover()
{
	FHitResult HitResult;
	return Super::CoverTrace(HitResult);
}

//Takes Actor forward vector and object normal to calculate from which side it enteres.
TEnumAsByte<ECoverSide> ASoldierEnemy::CheckSideByNormal(FVector Forward, FVector Normal)
{
	Forward.Normalize();
	Normal = - Normal;
	const float CosNormal = Normal.CosineAngle2D(FVector(0, 1, 0));
	const float CosForward = Forward.CosineAngle2D(FVector(0, 1, 0));
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

void ASoldierEnemy::CleanCoverData()
{
	CoverData.CoverType = CoverData.PendingCoverType = None;
	CoverData.CoverPart = CoverData.PendingCoverPart = CPNone;
	CoverData.CoverSide = CoverData.PendingCoverSide = CSNone;
	CoverData.IsTurning = CoverData.IsFiring = CoverData.IsSwitchingCoverType = CoverData.IsInCoverTransition = CoverData.IsInFireTransition = false;
	CoverData.CoverObject = nullptr;
}

int8 ASoldierEnemy::GetCoverIndex()
{
	if (CoverData.CoverType != ECoverType::None || CoverData.CoverPart != ECoverPart::CPNone || CoverData.CoverSide != ECoverSide::CSNone)
	return CoverData.CoverType * 2 + CoverData.CoverPart * 4 + CoverData.CoverSide;
	else return -1;
}

void ASoldierEnemy::ThrowGrenadeCaller()
{
	if (!CoverData.IsInTransition() && !CoverData.IsFiring && !bIsFiringBP && (GetCoverIndex() >= 2 || GetCoverIndex() == -1))
	{
		ThrowGrenadeDelegate.Broadcast();
		UE_LOG(LogPRAISoldier, Log, TEXT("BROADCASTED GRENADE"))
	}
}

void ASoldierEnemy::ThrowGrenade()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("THROW GRENADE ACTIVATED"))
	AbilitySystemComponent->TryActivateAbilityByClass(GameplayAbilities.FindRef(EGASInputActions::GrenadeThrow));
}

bool ASoldierEnemy::UsesOwnGrenades()
{
	return Grenades.Num() < 1 ? false : true;
}

bool ASoldierEnemy::SwitchGrenade()
{
	if (Grenades.Num() == 1) {return false;}
	int32 tmp;
	if (!Grenades.Find(CurrentGrenade, tmp)) {return false;}
	if (tmp == Grenades.Num() - 1)
	{
		CurrentGrenade = Grenades[0];
		return true;
	}
	else
	{
		CurrentGrenade = Grenades[tmp+1];
		return true;
	}
}

TSubclassOf<ABaseGrenade> ASoldierEnemy::GetCurrentGrenade()
{
	return CurrentGrenade;
}
