// Project Revival. All Rights Reserved

#define HIGHLIGHTABLE_TRACE_CHANNEL ECC_GameTraceChannel2
#define HIGHLIGHTABLE_COLLISION_OBJECT ECC_GameTraceChannel1


#include "Player/PlayerCharacter.h"
#include "AICharacter.h"
#include "DrawDebugHelpers.h"
#include "PRGameInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/BaseCharacterMovementComponent.h"
#include "GameFeature/StaticObjectToNothing.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "AbilitySystem/Abilities/DimensionShotAbility.h"
#include "Sound/SoundCue.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraSocket = CreateDefaultSubobject<USceneComponent>("CameraSocket");
	CameraSocket->SetupAttachment(RootComponent);
	CameraSocket->SetRelativeLocation(FVector(-30.0, 0.0, 30.0));
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(CameraSocket);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	PlayerMovementComponent = Cast<UBaseCharacterMovementComponent>(GetCharacterMovement());

	CoverData.SetOwner(this);
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this,&APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnAround",this,&APlayerCharacter::LookAround);
	PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed,PlayerMovementComponent, &UBaseCharacterMovementComponent::Jump);
	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Pressed,this, &APlayerCharacter::StartRun);
	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Released,this, &APlayerCharacter::StopRun);
	//PlayerInputComponent->BindAction("Flip",EInputEvent::IE_Pressed,this, &APlayerCharacter::Flip);
	//PlayerInputComponent->BindAction("Highlight",EInputEvent::IE_Pressed,this, &APlayerCharacter::HighlightAbility);
	PlayerInputComponent->BindAction("Fire",EInputEvent::IE_Pressed,this, &APlayerCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire",EInputEvent::IE_Released,WeaponComponent, &UWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon",EInputEvent::IE_Pressed,WeaponComponent, &UWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload",EInputEvent::IE_Pressed,WeaponComponent, &UWeaponComponent::Reload);
	PlayerInputComponent->BindAction("LeftCameraView", EInputEvent::IE_Pressed,this, &APlayerCharacter::OnCameraMove);
	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent,
	FGameplayAbilityInputBinds(FString("ConfirmTarget"),
	FString("CancelTarget"), FString("EGASInputActions")));
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &APlayerCharacter::CameraZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &APlayerCharacter::CameraZoomOut);
	//PlayerInputComponent->BindAction("ChangeWorld", EInputEvent::IE_Pressed,this, &APlayerCharacter::OnWorldChanged);
	PlayerInputComponent->BindAction("Cover", EInputEvent::IE_Pressed,this, &APlayerCharacter::Cover);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed,this, &APlayerCharacter::CoverCrouch);
	
}

void APlayerCharacter::MoveForward(float Amount)
{
	if (Amount==0.f) return;
	if (CoverData.IsInCover() || CoverData.IsInTransition() || CoverData.IsFiring) return;
	bIsMovingForward = Amount>0;
	PlayerMovementComponent->MoveForward(Amount);

}

void APlayerCharacter::MoveRight(float Amount) 
{
	if (Amount==0.f) return;
	if (CoverData.IsInTransition() || CoverData.IsFiring) return;
	if (CoverData.IsInCover())
	{
		if (!(CoverData.TryMoveInCover(Amount, this)))
		{
			if (CameraCover.bIsShift == false)
			{
				CameraCover.StartPos = SpringArmComponent->SocketOffset.Y;
				if (LeftSideView.CamPos == false) CameraCover.EndPos = SpringArmComponent->SocketOffset.Y + CameraCover.CoverYShift;
				else CameraCover.EndPos = SpringArmComponent->SocketOffset.Y - CameraCover.CoverYShift;
				CameraCover.bIsShift = true;
				CameraCover.IsShifting = true;
				CameraCoverFunctions->CameraCoverYShiftTimeline.PlayFromStart();
			}
			return;
		}
	}
	bIsMovingRight = Amount>0;
	PlayerMovementComponent->MoveRight(Amount);
}

void APlayerCharacter::StartRun()
{
	if (PlayerMovementComponent->GetPlayerMovementLogic().IsInJump() || PlayerMovementComponent->GetPlayerMovementLogic().IsPivotTargeted) return;
	if (CoverData.IsInCover()) return;
	bWantsToRun=true;
}

void APlayerCharacter::StopRun()
{
	bWantsToRun=false;
}

void APlayerCharacter::StartFire()
{
	if (CoverData.IsReadyToFire())
	{
		WeaponComponent->StartFire();
		return;
	}
	if (!PlayerMovementComponent->GetPlayerMovementLogic().IsPivotTargeted ||
		PlayerMovementComponent->GetPlayerMovementLogic().IsInJump()) return;
	WeaponComponent->StartFire();
}

void APlayerCharacter::LookUp(float Amount)
{
	AddControllerPitchInput(Amount);
}

void APlayerCharacter::LookAround(float Amount)
{
	AddControllerYawInput(Amount);
}

void APlayerCharacter::CoverCrouch()
{
	if (!CoverData.TrySwitchCoverType(this)) return;
	if (CameraCover.bIsShift == true)
	{
		CameraCover.StartPos = SpringArmComponent->SocketOffset.Y;
		if (LeftSideView.CamPos == false) CameraCover.EndPos = SpringArmComponent->SocketOffset.Y - CameraCover.CoverYShift;
		else CameraCover.EndPos = SpringArmComponent->SocketOffset.Y + CameraCover.CoverYShift;
		CameraCover.bIsShift = false;
		CameraCover.IsShifting = true;
		CameraCoverFunctions->CameraCoverYShiftTimeline.PlayFromStart();
	}
	CameraCoverFunctions->OnHighCoverLow(SpringArmComponent, CameraCoverFunctions, CameraCover);
}

void APlayerCharacter::Cover()
{
	if (PlayerMovementComponent->GetPlayerMovementLogic().IsInJump()) return;
	if (!(LeftSideView.Block == false && LeftSideView.IsMoving == false)) return;
	if (FMath::Abs(GetAimDelta().Yaw)>60.f) return;
	if (CoverData.IsInCover())
	{
		LeftSideView.bIsFirstTimeChange = false;
		StopCover_Internal();
		return;
	}
	FHitResult CoverHit;
	const ECoverType CoverType = CoverTrace(CoverHit);
	if (CoverType==ECoverType::None) return;
	LeftSideView.bIsFirstTimeChange = false;
	StartCover_Internal(CoverHit);
}

FRotator APlayerCharacter::GetAimDelta() const
{
	if (!GetController()) return FRotator();
	const auto CameraRotation = GetController()->K2_GetActorRotation();
	const auto PawnRotation = GetActorRotation();

	auto Delta = CameraRotation.Yaw-PawnRotation.Yaw;
	if (FMath::Abs<float>(Delta)>180)
	{
		Delta = Delta + 360.0*FMath::Sign<float>(Delta)*(-1.0);
	}

	return FRotator(CameraRotation.Pitch, Delta, 0.0f);
}

FCoverData& APlayerCharacter::GetCoverData()
{
	return CoverData;
}

void APlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}

void APlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
CheckCameraOverlap();
}

void APlayerCharacter::CheckCameraOverlap()
{
	const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(HideMesh);

	TArray<USceneComponent*> MeshChildren;
	GetMesh()->GetChildrenComponents(true, MeshChildren);

	for (auto MeshChild : MeshChildren)
	{
		const auto
		MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
		if (MeshChildGeometry)
		{
			MeshChildGeometry->SetOwnerNoSee(HideMesh);
		}
	}
}

void APlayerCharacter::Falling()
{
	if(DimensionShotAbStruct.IsInRevolverAim)
	{
		if(DimensionShotAbStruct.Ability)
		{
			DimensionShotAbStruct.Ability->InterruptAbility();
		}
		DimensionShotAbStruct.IsInRevolverAim=false;
	}
	CameraZoomOut();
	PlayerMovementComponent->JumpPressEnded();
	Super::Falling();
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	bWantsToRun=false;
	Super::Landed(Hit);
}

TArray<FAmmoData> APlayerCharacter::GetPlayerWeapons() const
{
	return WeaponComponent->GetAllWeapons();
}

bool APlayerCharacter::IsRunning() const
{
	return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

ECoverType APlayerCharacter::CheckCover()
{
	FHitResult HitResult;
	return CoverTrace(HitResult);
}

void APlayerCharacter::OnTurn()
{
	CameraCover.bIsTurning = true;
	OnCameraMove();
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OnEnergyValueChangedHandle.IsBound()) OnEnergyValueChangedHandle.Clear();
	CameraCollisionComponent->OnComponentBeginOverlap.Clear();
	CameraCollisionComponent->OnComponentEndOverlap.Clear();
	Super::EndPlay(EndPlayReason);
}

void APlayerCharacter::OnEnergyAttributeChanged(const FOnAttributeChangeData& Data)
{
	Super::OnEnergyAttributeChanged(Data);
	//Для подключения делегата необходимо получить объект типа APlayerCharacter и сделать OnEnergyValueChangedHandle.BindUObject(this, &тип_класса::название_метода);
	//Далее указанный метод будет вызываться автоматически при помощи следующей команды
	float MaxEnergy = AttributeSet->GetMaxEnergy();
	if (OnEnergyValueChangedHandle.IsBound()) OnEnergyValueChangedHandle.Execute(Data.NewValue/MaxEnergy);
	
}

void APlayerCharacter::OnCooldownExpired(const FActiveGameplayEffect& ExpiredEffect)
{
	Super::OnCooldownExpired(ExpiredEffect);
	
}

void APlayerCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();
	AbilitySystemComponent->CancelAbilities();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	if (DeathVFX)
	{
		UGameplayStatics::SpawnEmitterAttached(DeathVFX,RootComponent);
	}
	if (DeathAnimMontage)
	{
		
		const float Duration = PlayAnimMontage(DeathAnimMontage);
		if (Duration>0)
		{
			GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &APlayerCharacter::EnterDeathWorld, Duration);
		}
	}
	else EnterDeathWorld();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(CameraCollisionComponent);
	check(GetCharacterMovement());
	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnCameraCollisionEndOverlap);

	PlayerAimZoomFunctions = NewObject<UPlayerAimZoomFunctions>(this);
	LeftSideViewFunctions = NewObject<ULeftSideViewFunctions>(this);
	CameraCoverFunctions = NewObject<UCameraCoverFunctions>(this);

	//LeftSideView.YCenter = (SpringArmComponent->SocketOffset.Y + tan(CameraComponent->GetRelativeRotation().Yaw * PI / 180) * SpringArmComponent->TargetArmLength) * 2.0;

	FOnTimelineVector TimelineProgress;
	FOnTimelineFloat TimelineFieldOfView;
	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
	TimelineFieldOfView.BindUFunction(this, FName("TimelineFieldOfView"));
	PlayerAimZoomFunctions->CurveTimeline.AddInterpVector(PlayerAimZoom.CurveVector, TimelineProgress);
	PlayerAimZoomFunctions->CurveTimeline.AddInterpFloat(PlayerAimZoom.CurveFloat, TimelineFieldOfView);

	FOnTimelineVector TimelineLeftSideView;
	TimelineLeftSideView.BindUFunction(this, FName("TimelineLeftSideView"));
	LeftSideViewFunctions->LeftSideViewCurveTimeline.AddInterpVector(LeftSideView.LeftSideViewVector, TimelineLeftSideView);

	FOnTimelineVector TimelineCover;
	FOnTimelineFloat TimelineCoverFieldOfView;
	TimelineCover.BindUFunction(this, FName("TimelineCover"));
	TimelineCoverFieldOfView.BindUFunction(this, FName("TimelineCoverFieldOfView"));
	CameraCoverFunctions->CameraCoverTimeline.AddInterpVector(CameraCover.CoverVector, TimelineCover);
	CameraCoverFunctions->CameraCoverTimeline.AddInterpFloat(CameraCover.CoverFloat, TimelineCoverFieldOfView);

	FOnTimelineFloat TimelineCoverYShift;
	TimelineCoverYShift.BindUFunction(this, FName("TimelineCoverYShift"));
	CameraCoverFunctions->CameraCoverYShiftTimeline.AddInterpFloat(CameraCover.CoverYShiftCurve, TimelineCoverYShift);

	FOnTimelineFloat TimelineCoverLow;
	TimelineCoverLow.BindUFunction(this, FName("TimelineCoverLow"));
	CameraCoverFunctions->CameraCoverLowTimeline.AddInterpFloat(CameraCover.CoverHighCoverCrouchCurve, TimelineCoverLow);

	CameraCover.SavePosRight = SpringArmComponent->SocketOffset;
	CameraCover.SavePosLeft = SpringArmComponent->SocketOffset;
	CameraCover.SavePosLeft.Y = SpringArmComponent->SocketOffset.Y - (SpringArmComponent->SocketOffset.Y + tan(CameraComponent->GetRelativeRotation().Yaw * PI / 180) * SpringArmComponent->TargetArmLength) * 2.f;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerAimZoomFunctions->CurveTimeline.TickTimeline(DeltaTime);
	LeftSideViewFunctions->LeftSideViewCurveTimeline.TickTimeline(DeltaTime);
	CameraCoverFunctions->CameraCoverTimeline.TickTimeline(DeltaTime);
	CameraCoverFunctions->CameraCoverYShiftTimeline.TickTimeline(DeltaTime);
	CameraCoverFunctions->CameraCoverLowTimeline.TickTimeline(DeltaTime);
}


void APlayerCharacter::TimelineProgress(float Value)
{
	PlayerAimZoomFunctions->TimelineProgress(Value, PlayerAimZoom);
}


void APlayerCharacter::TimelineFieldOfView(float Value)
{
	PlayerAimZoomFunctions->TimelineFieldOfView(Value, PlayerAimZoom);
}

void APlayerCharacter::TimelineLeftSideView(float Value)
{
	LeftSideViewFunctions->TimelineLeftSideView(Value, LeftSideView, PlayerAimZoom);
}


void APlayerCharacter::TimelineCover(float Value)
{
	CameraCoverFunctions->TimelineCover(Value, CameraCoverFunctions, SpringArmComponent);
}


void APlayerCharacter::TimelineCoverLow(float Value)
{
	CameraCoverFunctions->TimelineCoverLow(Value, CameraCoverFunctions, SpringArmComponent);
}

void APlayerCharacter::SetChangeWorldPossibility(bool newValue, AStaticObjectToNothing* overlappedAct)
{
	OverlappedChangeWActor=overlappedAct;
	WorldCanBeChanged=newValue;
}

void APlayerCharacter::SetChangeWorldPossibility(bool newValue, ASoldierEnemy* overlappedAct)
{
	OverlappedChangeWEnemy = overlappedAct;
	WorldCanBeChanged = newValue;
}

bool APlayerCharacter::CheckIfWorldCanBeChanged() const
{
	if(!WorldCanBeChanged)
	{
		if(WorldCantBeChangedPhrase)
			UGameplayStatics::SpawnSound2D(GetWorld(),WorldCantBeChangedPhrase);
		if(OverlappedChangeWActor)
		{
			OverlappedChangeWActor->ShowChangeWorldObjectByAbility();
		}
		if(OverlappedChangeWEnemy)
		{
			OverlappedChangeWEnemy->ShowChangeWorldObjectByAbility();
		}
	}
	return WorldCanBeChanged;
}


void APlayerCharacter::TimelineCoverFieldOfView(float Value)
{
	CameraCoverFunctions->TimelineCoverFieldOfView(Value, CameraComponent, CameraCover);
}

void APlayerCharacter::TimelineCoverYShift(float Value)
{
	CameraCoverFunctions->TimelineCoverYShift(Value, SpringArmComponent, CameraCover);
}


void APlayerCharacter::CameraZoomIn()
{
	if (LeftSideView.IsMoving == false && PlayerAimZoom.IsZooming==false && !(CameraCoverFunctions->CameraCoverYShiftTimeline.IsPlaying()))
	{
		if (CoverData.IsInCover() && CameraCoverFunctions->CoverType == ECoverType::High && CameraCover.bIsShift == false) return;
		if (CoverData.IsInTransition()) return;
		if (CoverData.IsInCover() && !CoverData.IsInCoverTransition)
		{
			CoverData.CoverToAim();
		}
		if (PlayerMovementComponent->GetPlayerMovementLogic().IsInJump() || PlayerMovementComponent->GetPlayerMovementLogic().IsPivotTargeted) return;
		if (!CoverData.IsInCover())
		{
			bWantsToRun=false;
			PlayerMovementComponent->bOrientRotationToMovement = 0;
			PlayerMovementComponent->AimStart();
		}
		bUseControllerRotationYaw=true;
		PlayerAimZoomFunctions->CameraZoomIn(SpringArmComponent, LeftSideView, PlayerAimZoom, CameraComponent, PlayerAimZoomFunctions->CurveTimeline, CoverData, CameraCover, CameraCoverFunctions);
	}
}

void APlayerCharacter::CameraZoomOut()
{
	if (LeftSideView.IsMoving == false && PlayerAimZoom.IsZooming == true && !DimensionShotAbStruct.IsInRevolverAim)
	{
		if (CoverData.IsInTransition()) return;
		if (CoverData.IsInCover() && CoverData.IsFiring)
		{
			CoverData.AimToCover();
		}
		if (!CoverData.IsInCover())
		{
			PlayerMovementComponent->bOrientRotationToMovement = 1;
			PlayerMovementComponent->AimEnd();
		}
		bUseControllerRotationYaw=false;
		PlayerAimZoomFunctions->CameraZoomOut(SpringArmComponent, PlayerAimZoomFunctions->CurveTimeline, PlayerAimZoom, CoverData, CameraCover);
	}
}


void APlayerCharacter::OnCameraMove()
{
	if (CoverData.IsInTransition()) return;
	if (LeftSideView.Block == false && PlayerAimZoom.IsZooming == false && LeftSideView.IsMoving == false && !(CameraCoverFunctions->CameraCoverYShiftTimeline.IsPlaying()))
	{
		if (CoverData.IsInCover() && CameraCover.bIsTurning == false) return;
		LeftSideViewFunctions->OnCameraMove(SpringArmComponent, CameraComponent, LeftSideView, LeftSideViewFunctions->LeftSideViewCurveTimeline, CameraCover, CoverData, CameraCoverFunctions);
	}
}


void APlayerCharacter::OnWorldChanged()
{
	TSubclassOf<AChangeWorld> ClassToFind = AStaticObjectToNothing::StaticClass();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, ClassToFind, OutActors);
	for (int EveryActor = 0; EveryActor < OutActors.Num(); EveryActor++)
	{
		Cast<AStaticObjectToNothing>(OutActors[EveryActor])->Changing();
	}
}


bool APlayerCharacter::StartCover_Internal(FHitResult& CoverHit)
{
	const bool Sup = Super::StartCover_Internal(CoverHit);
	if (!Sup)return false;
	CameraZoomOut();

	if (LeftSideView.CamPos == false) SpringArmComponent->SocketOffset = CameraCover.SavePosRight;
	else SpringArmComponent->SocketOffset = CameraCover.SavePosLeft;
	CameraCoverFunctions->Start = SpringArmComponent->SocketOffset;
	CameraCoverFunctions->End = SpringArmComponent->SocketOffset + CameraCover.CameraCover;
	CameraCoverFunctions->CoverType = CoverTrace(CoverHit);
	if (CameraCoverFunctions->CoverType == ECoverType::Low) CameraCoverFunctions->End.Z -= CameraCover.Low;
	if (LeftSideView.CamPos == true) CameraCoverFunctions->End.Y += 104.0;
	PlayerAimZoom.StartStartPos = FVector(0.0);
	CameraCover.CurrentFieldOfView = CameraCover.FieldOfView;
	CameraCover.IsFirstLow = 0;
	CameraCover.IsFirstHigh = 0;
	CameraCoverFunctions->CameraCoverTimeline.PlayFromStart();
	
	bWantsToRun = false;
	WeaponComponent->StopFire();
	PlayerMovementComponent->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = false;
	AdjustLocationBeforeCover(CoverHit);
	CoverData.StartCover(FMath::Sign(SpringArmComponent->SocketOffset.Y), 0, CheckCover(), CoverHit.GetActor());
	return true;
}

bool APlayerCharacter::StopCover_Internal()
{
	const bool Sup = Super::StopCover_Internal();
	if (!Sup)return false;

	LeftSideView.SavePosLeft = FVector(0.0);
	LeftSideView.SavePosRight = FVector(0.0);

	CameraCoverFunctions->Start = SpringArmComponent->SocketOffset;
	CameraCoverFunctions->End = SpringArmComponent->SocketOffset - CameraCover.CameraCover;
	if (CameraCover.bIsLow == true) { CameraCoverFunctions->End.Z += CameraCover.Low; CameraCover.bIsLow = false; }
	if (CameraCoverFunctions->CoverType == ECoverType::Low) CameraCoverFunctions->End.Z += CameraCover.Low;
	if (LeftSideView.CamPos == true) CameraCoverFunctions->End.Y -= 104.0;
	CameraCoverFunctions->CoverType = None;
	PlayerAimZoom.StartStartPos = FVector(0.0);
	CameraCover.CurrentFieldOfView = 90.0;

	if (CameraCover.bIsShift == true)
	{
		if (LeftSideView.CamPos == false) CameraCoverFunctions->End.Y -= CameraCover.CoverYShift;
		else CameraCoverFunctions->End.Y += CameraCover.CoverYShift;
		CameraCover.bIsShift = false;
	}
	CameraCoverFunctions->CameraCoverTimeline.PlayFromStart();
	
	CoverData.StopCover();
	PlayerMovementComponent->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	return true;
}

void APlayerCharacter::EnterDeathWorld()
{
	GetWorldTimerManager().ClearTimer(DeathTimerHandle);
	UGameplayStatics::OpenLevel(this,/*GameInstance->GetStartupLevel().LevelName*/GetGameInstance<UPRGameInstance>()->GetDeathWorldLevelName(),true);
}
