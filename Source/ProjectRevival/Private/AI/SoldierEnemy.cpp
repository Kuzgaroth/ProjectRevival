// Project Revival. All Rights Reserved

#define COVER_TRACE_CHANNEL ECC_GameTraceChannel3


#include "AI/SoldierEnemy.h"
#include "AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIWeaponComponent.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "HealthBarWidget.h"
#include "PlayerCharacter.h"
#include "SoldierRifleWeapon.h"
#include "AbilitySystem/Abilities/GrenadeAbility.h"
#include "AbilitySystem/AbilityActors/ChangeWorldSphereActor.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "GameFeature/ChangeWorld.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

	InterpFunction.BindUFunction(this,FName("TimeLineFloatReturn"));
	OnTimeLineFinished.BindUFunction(this,FName("TimeLineFinished"));
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

void ASoldierEnemy::SetBIsAppearing(bool const bCond)
{
	bIsAppearing = bCond;
	UE_LOG(LogPRAISoldier, Warning, TEXT("bIsAppearing updated in Soldier: %s"), bIsAppearing?TEXT("true"):TEXT("false"))
	SoldierWorldChangeDelegate.Broadcast(GetBIsAppearing());
}

void ASoldierEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ASoldierAIController* SoldierController = Cast<ASoldierAIController>(NewController);
	PlayerCoordinates = SoldierController->GetPlayerPos();
	UpdateAimRotator();
	DrawDebugLine(GetWorld(),GetActorLocation(),PlayerCoordinates.GetActorPosition(),FColor::Purple,false,5.0f,0,3.0f);
	SoldierController->StartEnteringCoverDelegate.AddDynamic(this, &ASoldierEnemy::StartCoverSoldier);
	SoldierController->StartExitingCoverDelegate.AddDynamic(this, &ASoldierEnemy::StopCoverSoldier);
	SoldierController->StartCoverSideMovingDelegate.AddDynamic(this, &ASoldierEnemy::ChangeCoverSide);
	SoldierController->StartFiringDelegate.AddDynamic(this, &ASoldierEnemy::StartFiring);
	SoldierController->PlayerPosDelegate.AddDynamic(this, &ASoldierEnemy::UpdatePlayerCoordinates);

	SkeletalMesh = GetMesh();
	CapsuleComp = GetCapsuleComponent();
	
	SetBIsAppearing(CurrentWorld == World);
	
	int num = SkeletalMesh->GetNumMaterials();
	for (int32 i=0; i < num; i++)
	{
		const auto Material = SkeletalMesh->CreateDynamicMaterialInstance(i);
		MeshesMaterials.Add(Material);
	}
	if(VisualCurve)
	{
		VisualCurve->GetValueRange(MinCurveValue,MaxCurveValue);
		TimeLine.AddInterpFloat(VisualCurve,InterpFunction);
		TimeLine.SetTimelineFinishedFunc(OnTimeLineFinished);
		TimeLine.SetLooping(false);
	}
	if (World==OrdinaryWorld)
	{
		SetBIsAppearing(true);
		if(VisualCurve&&MeshesMaterials.Num()>0)
		{
			for (const auto Material : MeshesMaterials)
			{
				Material->SetScalarParameterValue("Amount",MinCurveValue);
			}
		}
		else
		{
			SkeletalMesh->SetVisibility(true);
		}
		SetCollisionResponseToVisible();
		SkeletalMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
	}
	else
	{
		if(VisualCurve&&MeshesMaterials.Num()>0)
		{
			for (const auto Material : MeshesMaterials)
			{
				Material->SetScalarParameterValue("Amount",MaxCurveValue);
			}
		}
		else
		{
			SkeletalMesh->SetVisibility(false);
		}
		SetCollisionResponseToInvisible();
		SkeletalMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
	}
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this,&ASoldierEnemy::OnMeshComponentCollision);
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
		UE_LOG(LogTemp, Log, TEXT("BBComp was updated %i"), EnumKey)
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
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverSoldier() was called"))
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FCollisionQueryParams CollisionParams;
	CollisionParams.bIgnoreTouches = true;
	CollisionParams.AddIgnoredActor(this);
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: CoverPos: %s"), *CoverPos.ToString())
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: CoverRef name is %s"), *CoverRef->GetName())
	CoverData.CoverObject = CoverRef;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), CoverPos, CoverRef->GetActorLocation(), 300,
		ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);
	int8 IndexFound = -1;
	for (int8 i = 0; i < HitResults.Num(); i++)
	{
		if (CoverRef->GetRootComponent() == HitResults[i].GetComponent()) 
		{
			IndexFound = i;
			break;
		}
	}
	if (IndexFound == -1)
	{
		CleanCoverData();
		return;
	}
	
	StopFiringImmediately();

	CoverData.CoverObject = CoverRef;
	CoverData.CoverObject->ActorHasTag(FName(TEXT("High"))) ? CoverData.CoverType = High : CoverData.CoverType = Low;
	CoverData.CoverSide = Right;
	CoverData.CoverPart = GetCoverPart(0);
	
	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	GetCharacterMovement()->SetPlaneConstraintNormal(HitResults[IndexFound].Normal);
	bUseControllerRotationYaw = false;
	
	FRotator RotationToCover = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResults[IndexFound].ImpactPoint);
	RotationToCover.Pitch = 0.f;
	RotationToCover.Roll = 0.f;
	RotationToCover.Yaw = RotationToCover.Yaw - GetActorRotation().Yaw;
	// DrawDebugLine(GetWorld(),HitResults[IndexFound].ImpactPoint,GetActorLocation(),FColor::Orange,false,15.0f,0,3.0f);
	// DrawDebugLine(GetWorld(),GetActorLocation(),GetActorLocation()+GetActorForwardVector()*100,FColor::Purple,false,15.0f,0,3.0f);
	AddActorLocalRotation(RotationToCover);
	CoverData.IsInCoverTransition = true;
	// StartEnteringCoverForAnimDelegate.Broadcast();
}

void ASoldierEnemy::StartCoverSoldierFinish()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverSoldierFinish() was called"));
	// DrawDebugLine(GetWorld(),GetActorLocation(),GetActorLocation()+GetActorForwardVector()*100,FColor::Purple,false,15.0f,0,3.0f);
	CoverData.IsInCoverTransition = false;
	bIsInCoverBP = true;
	StopEnteringCoverDelegate.Broadcast();
}

void ASoldierEnemy::StopCoverSoldier()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StopCoverSoldier() was called"))
	if (bIsFiringBP)
	{
		UE_LOG(LogPRAISoldier, Log, TEXT("Character: StopCoverSoldier() at firing"))
		StopFiringImmediately();
		GetCharacterMovement()->SetPlaneConstraintEnabled(false);
		bUseControllerRotationYaw = false;
		CoverData.StopCover();
	}
	else
	{
		UE_LOG(LogPRAISoldier, Log, TEXT("Character: StopCoverSoldier() at common"))
		GetCharacterMovement()->SetPlaneConstraintEnabled(false);
		bUseControllerRotationYaw = false;
		CoverData.StopCover();
	}
}

void ASoldierEnemy::StopCoverSoldierFinish()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StopCoverSoldierFinish() was called"))
	CleanCoverData();
	bIsInCoverBP = false;
	StopExitingCoverDelegate.Broadcast();
}

void ASoldierEnemy::ChangeCoverSide(const float Amount)
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: ChangeCoverSide() was called"))
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
		}
	}
}

void ASoldierEnemy::ChangeCoverSideFinish()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: ChangeCoverSideFinish() was called"))
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
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: ChangeCoverTypeFinish() was called"))
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
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverToFire() was called"))
	if (GetCoverIndex() < 2) {return;}
	CoverData.IsInFireTransition = true;
}

void ASoldierEnemy::StartCoverToFireFinish()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverToFireFinish() was called"))
	CoverData.IsInFireTransition = false;
	CoverData.IsFiring = true;
	StartFiring();
}

void ASoldierEnemy::StartCoverFromFire()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverFromFire() was called"))
	if (!CoverData.IsInCover()) {return;}
	CoverData.IsFiring = false;
	CoverData.IsInFireTransition = true;
}

void ASoldierEnemy::StartCoverFromFireFinish()
{	
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartCoverFromFireFinish() was called"))
	CoverData.IsInFireTransition = false;
	StopFiring();
}

void ASoldierEnemy::StartFiring()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: Bot doing this is %s"), *GetName())
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() was called"))
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() bIsInCoverBP               is %s"), bIsInCoverBP ? TEXT("true") : TEXT("false"))
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() CoverData.IsFiring         is %s"), CoverData.IsFiring ? TEXT("true") : TEXT("false"))
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() CoverData.IsInTransition() is %s"), CoverData.IsInTransition() ? TEXT("true") : TEXT("false"))
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() bIsFiringBP                is %s"), bIsFiringBP ? TEXT("true") : TEXT("false"))
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() GetCoverIndex              is %d"), GetCoverIndex())
	if (bIsInCoverBP && !CoverData.IsFiring && !bIsFiringBP && !CoverData.IsInTransition() && GetCoverIndex() >= 2)
	{
		StartCoverToFire();
		return;
	}
	else if (bIsInCoverBP && CoverData.IsFiring && !CoverData.IsInTransition() && !bIsFiringBP)
	{
		UE_LOG(LogPRAISoldier, Log, TEXT("Character: Did we come here? V1"))
		UpdateAimRotator();
		DrawDebugLine(GetWorld(),GetActorLocation(),PlayerCoordinates.GetActorPosition(),FColor::Purple,false,5.0f,0,3.0f);
		if (Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon()))
		{
			UE_LOG(LogPRAISoldier, Log, TEXT("Character: Passed RowRifle Cast<> V1"))
			UE_LOG(LogPRAISoldier, Log, TEXT("Character: CurrentWeapon is %s"), *WeaponComponent->GetCurrentWeapon()->GetName())
			RowRifleRef = Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon());
			RowRifleRef->StoppedFireInWeaponDelegate.AddDynamic(this, &ASoldierEnemy::StopFiring);
			RowRifleRef->OnWeaponShotDelegate.AddUObject(this, &ASoldierEnemy::UpdateAimRotator);
			UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() StoppedFireInWeaponDelegate IsBound %s"), RowRifleRef->StoppedFireInWeaponDelegate.IsBound() ? TEXT("true") : TEXT("false"))
			UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() OnWeaponShotDelegate        IsBound %s"), RowRifleRef->OnWeaponShotDelegate.IsBound() ? TEXT("true") : TEXT("false"))
			RowRifleRef->StartFire();
		}
		else
		{
			WeaponComponent->StartFire();
		}
		bIsFiringBP = true;
		return;
	}// The following line exists in case we would need different logic for firing in and out of cover. But for now it works fine like that.
	else if (!bIsInCoverBP && !CoverData.IsInTransition() && !bIsFiringBP)
	{
		UE_LOG(LogPRAISoldier, Log, TEXT("Character: Did we come here? V2"))
		UpdateAimRotator();
		DrawDebugLine(GetWorld(),GetActorLocation(),PlayerCoordinates.GetActorPosition(),FColor::Purple,false,5.0f,0,3.0f);
		if (Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon()))
		{
			UE_LOG(LogPRAISoldier, Log, TEXT("Character: Passed RowRifle Cast<> V2"))
			UE_LOG(LogPRAISoldier, Log, TEXT("Character: CurrentWeapon is %s"), *WeaponComponent->GetCurrentWeapon()->GetName())
			RowRifleRef = Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon());
			RowRifleRef->StoppedFireInWeaponDelegate.AddDynamic(this, &ASoldierEnemy::StopFiring);
			UpdateRowRifleDelegateHandle = RowRifleRef->OnWeaponShotDelegate.AddUObject(this, &ASoldierEnemy::UpdateAimRotator);
			UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() StoppedFireInWeaponDelegate IsBound %s"), RowRifleRef->StoppedFireInWeaponDelegate.IsBound() ? TEXT("true") : TEXT("false"))
			UE_LOG(LogPRAISoldier, Log, TEXT("Character: StartFiring() OnWeaponShotDelegate        IsBound %s"), RowRifleRef->OnWeaponShotDelegate.IsBound() ? TEXT("true") : TEXT("false"))
			RowRifleRef->StartFire();
		}
		else
		{
			WeaponComponent->StartFire();
		}
		bIsFiringBP = true;
		return;
	}
}

//It works properly for bots only if their weapon have StoppedFireInWeaponDelegate.
//Lately every logic inside should follow the RowRifleRef logic style.
void ASoldierEnemy::StopFiring()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StopFiring() was called"))
	if (bIsInCoverBP && CoverData.IsFiring && bIsFiringBP)
	{
		StartCoverFromFire();
		return;
	}
	else if (!CoverData.IsFiring && bIsFiringBP)
	{
		if (Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon()))
		{
			RowRifleRef->StoppedFireInWeaponDelegate.RemoveDynamic(this, &ASoldierEnemy::StopFiring);
			if (RowRifleRef->OnWeaponShotDelegate.IsBound())
			{
				RowRifleRef->OnWeaponShotDelegate.Remove(UpdateRowRifleDelegateHandle);
			}
			RowRifleRef = nullptr;
		}
		else
		{
			WeaponComponent->StopFire();
		}
		bIsFiringBP = false;
		StopFireDelegate.Broadcast();
		return;
	}
	// else if (CoverData.IsInCover() && bIsInCoverBP && !CoverData.IsFiring && bIsFiringBP)
	// {
	// 	
	// }
	// else if (!CoverData.IsInCover() && !bIsInCoverBP && bIsFiringBP)
	// {
	// 	
	// }
}

void ASoldierEnemy::ChangeVisibleWorld(EChangeAllMapEditorVisibility VisibleInEditorWorld)
{
	if(VisibleInEditorWorld!=OwnValuesWorld)
	{
		switch (VisibleInEditorWorld)
		{
		case DefaultVisibleWorld:
			VisibleWorld=DefaultWorld;
			break;
		case OtherVisibleWorld:
			VisibleWorld=AltirnativeWorld;
			break;
		case BothVisibleWorlds:
			VisibleWorld=BothWorlds;
			break;
		default:
			break;
		}
		if(VisibleWorld==World || VisibleWorld==BothWorlds)
		{
			SkeletalMesh->SetVisibility(true);
		}
		else
		{
			SkeletalMesh->SetVisibility(false);
		}
	}
	else
	{
		VisibleWorld=DefaultWorld;
		if(VisibleWorld==World || VisibleWorld==BothWorlds)
		{
			SkeletalMesh->SetVisibility(true);
		}
		else
		{
			SkeletalMesh->SetVisibility(false);
		}
	}
}

void ASoldierEnemy::Changing()
{
	UE_LOG(LogPRAISoldier, Warning, TEXT("Changing func is called"))
	if(CurrentWorld==OrdinaryWorld)
	{
		CurrentWorld=OtherWorld;
	}
	else
	{
		CurrentWorld=OrdinaryWorld;
	}
	if (World == CurrentWorld)
	{
		SetCollisionResponseToVisible();
		SetBIsAppearing(true);
		if(VisualCurve)
		{
			CapsuleComp->SetCollisionResponseToChannels(CollisionResponseContainer);
			SkeletalMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
			SkeletalMesh->SetVisibility(true);
			Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon())->GetWeaponMeshComponent()->SetVisibility(true);
			TimeLine.PlayFromStart();
		}
		else
		{
			CapsuleComp->SetCollisionResponseToChannels(CollisionResponseContainer);
			SkeletalMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
			SkeletalMesh->SetVisibility(true);
			Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon())->GetWeaponMeshComponent()->SetVisibility(true);
		}
	}
	else
	{
		SetCollisionResponseToInvisible();
		StopFiringImmediately();
		SetBIsAppearing(false);
		if(VisualCurve)
		{
			CapsuleComp->SetCollisionResponseToChannels(CollisionResponseContainer);
			SkeletalMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
			SkeletalMesh->SetVisibility(false);
			Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon())->GetWeaponMeshComponent()->SetVisibility(false);
			TimeLine.PlayFromStart();
		}
		else
		{
			CapsuleComp->SetCollisionResponseToChannels(CollisionResponseContainer);
			SkeletalMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
			SkeletalMesh->SetVisibility(false);
			Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon())->GetWeaponMeshComponent()->SetVisibility(false);
		}
		if (bIsInCoverBP)
		{
			StopCoverSoldier();
		}
	}
}

#if WITH_EDITOR
void ASoldierEnemy::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(PropertyChangedEvent.Property->GetName()=="VisibleWorld")
	{
		if(VisibleWorld==World || VisibleWorld==BothWorlds)
		{
			SkeletalMesh->SetVisibility(true);
		}
		else
		{
			SkeletalMesh->SetVisibility(false);
		}
	}
	if(PropertyChangedEvent.Property->GetName()=="AllObjectVisibleWorld")
	{
		TArray<AActor*> ChangeAbleObjs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),AChangeWorld::StaticClass(),ChangeAbleObjs);
		for(auto obj:ChangeAbleObjs)
		{
			Cast<ASoldierEnemy>(obj)->ChangeVisibleWorld(AllObjectVisibleWorld);
		}
	}
}
#endif

void ASoldierEnemy::ShowChangeWorldObjectByAbility()
{
	SkeletalMesh->SetRenderCustomDepth(true);
	if(CapsuleComp->GetCollisionResponseToChannel(ECC_WorldDynamic) == ECR_Overlap)
	{
		if(MeshesMaterials.Num()!=0)
			for (const auto Material : MeshesMaterials)
			{
				auto reqwar=(MaxCurveValue-MinCurveValue)/TransparencyLevel;
				reqwar=MaxCurveValue-reqwar;
				Material->SetScalarParameterValue("Amount",reqwar);
			}
	}
}

void ASoldierEnemy::HideChangeWorldObjectByAbility()
{
	SkeletalMesh->SetRenderCustomDepth(false);
	if(CapsuleComp->GetCollisionResponseToChannel(ECC_WorldDynamic) == ECR_Overlap)
	{
		if(MeshesMaterials.Num()!=0)
			for (const auto Material : MeshesMaterials)
			{
				Material->SetScalarParameterValue("Amount",MaxCurveValue);
			}
	}
}

void ASoldierEnemy::OnMeshComponentCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogPRAISoldier, Warning, TEXT("OnMeshComponentCollision"))
	UE_LOG(LogPRAISoldier, Warning, TEXT("%s"), *FString(OtherActor->GetName()))
	if(Cast<AChangeWorldSphereActor>(OtherActor))
	{
		UE_LOG(LogPRAISoldier, Warning, TEXT("Cast went successful"))
		Changing();
	}
	auto Player=Cast<APlayerCharacter>(OtherActor);
	if(Player&& CapsuleComp->GetCollisionResponseToChannel(ECC_WorldDynamic) == ECR_Overlap)
	{
		Player->SetChangeWorldPossibility(false,this);
	}
}

void ASoldierEnemy::OnMeshComponentEndCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		ASoldierEnemy* ptr = nullptr;
		Player->SetChangeWorldPossibility(true, ptr);
		HideChangeWorldObjectByAbility();
	}
}

//This function is only for inner usage.
//Called when we want to abort covering during firing. 
void ASoldierEnemy::StopFiringImmediately()
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: StopFiringImmediately was called"))
	if (Cast<ASoldierRifleWeapon>(WeaponComponent->GetCurrentWeapon()))
	{
		if (!RowRifleRef) { return; }
		RowRifleRef->StopFireExternal();
		if (RowRifleRef->StoppedFireInWeaponDelegate.IsBound())
		{
			RowRifleRef->StoppedFireInWeaponDelegate.RemoveDynamic(this, &ASoldierEnemy::StopFiring);
		}
		if (RowRifleRef->OnWeaponShotDelegate.IsBound())
		{
			RowRifleRef->OnWeaponShotDelegate.Remove(UpdateRowRifleDelegateHandle);
		}
		RowRifleRef = nullptr;
	}
	else
	{
		WeaponComponent->StopFire();
	}
	CoverData.IsFiring = false;
	bIsFiringBP = false;
	StopFireDelegate.Broadcast();
}

TArray<FAmmoData> ASoldierEnemy::GetPlayerWeapons() const
{
	return WeaponComponent->GetAllWeapons();
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
	return (CosNormal >= CosForward)?Left:Right;
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

void ASoldierEnemy::UpdatePlayerCoordinates(const FPlayerPositionData& PlayerPos)
{
	UE_LOG(LogPRAISoldier, Log, TEXT("Character: UpdatePlayerCoordinates was called"))
	PlayerCoordinates = PlayerPos;
	UpdateAimRotator();
}

void ASoldierEnemy::UpdateAimRotator()
{
	AimRotator = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),PlayerCoordinates.GetActorPosition()), GetActorRotation());
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
	bool const bCond = CoverData.CoverType != ECoverType::None || CoverData.CoverPart != ECoverPart::CPNone || CoverData.CoverSide != ECoverSide::CSNone;
	
	return bCond ? CoverData.CoverType * 2 + CoverData.CoverPart * 4 + CoverData.CoverSide : -1;
}

void ASoldierEnemy::SetCollisionResponseToVisible()
{
	CollisionResponseContainer.SetResponse(ECC_WorldStatic, ECR_Block);
	CollisionResponseContainer.SetResponse(ECC_WorldDynamic, ECR_Block);
	CollisionResponseContainer.SetResponse(ECC_Pawn, ECR_Block);
	CollisionResponseContainer.SetResponse(ECC_PhysicsBody, ECR_Block);
	CollisionResponseContainer.SetResponse(ECC_Vehicle, ECR_Block);
	CollisionResponseContainer.SetResponse(ECC_Destructible, ECR_Block);
}

void ASoldierEnemy::SetCollisionResponseToInvisible()
{
	CollisionResponseContainer.SetResponse(ECC_WorldStatic, ECR_Block);
	CollisionResponseContainer.SetResponse(ECC_WorldDynamic, ECR_Overlap);
	CollisionResponseContainer.SetResponse(ECC_Pawn, ECR_Overlap);
	CollisionResponseContainer.SetResponse(ECC_PhysicsBody, ECR_Overlap);
	CollisionResponseContainer.SetResponse(ECC_Vehicle, ECR_Overlap);
	CollisionResponseContainer.SetResponse(ECC_Destructible, ECR_Overlap);
}

void ASoldierEnemy::TimeLineFinished()
{
	IIChangingWorldActor::TimeLineFinished();
}

void ASoldierEnemy::TimeLineFloatReturn(float Value)
{
	for (const auto Material : MeshesMaterials)
	{
		if(GetBIsAppearing())
		{
			Material->SetScalarParameterValue("Amount",Value);
		}
		else
		{
			float val=MinCurveValue-Value;
			val=MaxCurveValue+val;
			Material->SetScalarParameterValue("Amount",val);
		}
	}
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
