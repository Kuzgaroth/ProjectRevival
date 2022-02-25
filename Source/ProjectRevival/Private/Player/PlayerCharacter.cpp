// Project Revival. All Rights Reserved

#define HIGHLIGHTABLE_TRACE_CHANNEL ECC_GameTraceChannel2
#define HIGHLIGHTABLE_COLLISION_OBJECT ECC_GameTraceChannel1


#include "Player/PlayerCharacter.h"
#include "AICharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/BaseCharacterMovementComponent.h"
#include "ProjectRevival/ProjectRevival.h"
#include "GameFeature/StaticObjectToNothing.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraSocket = CreateDefaultSubobject<USceneComponent>("CameraSocket");
	CameraSocket->SetupAttachment(RootComponent);
	
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
	PlayerInputComponent->BindAction("ChangeWorld", EInputEvent::IE_Pressed,this, &APlayerCharacter::OnWorldChanged);
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
		if (!(CoverData.TryMoveInCover(Amount, this))) return;;
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
	CoverData.TrySwitchCoverType(this);
}

void APlayerCharacter::Cover()
{
	if (PlayerMovementComponent->GetPlayerMovementLogic().IsInJump()) return;
	if (!(LeftSideView.Block == false && LeftSideView.IsMoving == false)) return;
	if (FMath::Abs(GetAimDelta().Yaw)>60.f) return;
	if (CoverData.IsInCover())
	{
		StopCover_Internal();
		return;
	}
	FHitResult CoverHit;
	const ECoverType CoverType = CoverTrace(CoverHit);
	if (CoverType==ECoverType::None) return;
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
	CameraZoomOut();
	PlayerMovementComponent->JumpPressEnded();
	Super::Falling();
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	bWantsToRun=false;
	Super::Landed(Hit);
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
	Super::OnDeath();
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
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

	FOnTimelineVector TimelineProgress;
	FOnTimelineFloat TimelineFieldOfView;
	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
	TimelineFieldOfView.BindUFunction(this, FName("TimelineFieldOfView"));
	PlayerAimZoomFunctions->CurveTimeline.AddInterpVector(PlayerAimZoom.CurveVector, TimelineProgress);
	PlayerAimZoomFunctions->CurveTimeline.AddInterpFloat(PlayerAimZoom.CurveFloat, TimelineFieldOfView);

	FOnTimelineFloat TimelineLeftSideView;
	TimelineLeftSideView.BindUFunction(this, FName("TimelineLeftSideView"));
	LeftSideViewFunctions->LeftSideViewCurveTimeline.AddInterpFloat(PlayerAimZoom.CurveFloat, TimelineLeftSideView);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerAimZoomFunctions->CurveTimeline.TickTimeline(DeltaTime);
	LeftSideViewFunctions->LeftSideViewCurveTimeline.TickTimeline(DeltaTime);
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

void APlayerCharacter::CameraZoomIn()
{
	if (LeftSideView.IsMoving == false && PlayerAimZoom.IsZooming==false)
	{
		
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
			bUseControllerRotationYaw=true;
			PlayerMovementComponent->AimStart();
		}
		
		PlayerAimZoomFunctions->CameraZoomIn(SpringArmComponent, LeftSideView, PlayerAimZoom, CameraComponent, PlayerAimZoomFunctions->CurveTimeline);
	}
}

void APlayerCharacter::CameraZoomOut()
{
	if (LeftSideView.IsMoving == false && PlayerAimZoom.IsZooming == true)
	{
		if (CoverData.IsInTransition()) return;
		if (CoverData.IsInCover() && CoverData.IsFiring)
		{
			CoverData.AimToCover();
		}
		if (!CoverData.IsInCover())
		{
			PlayerMovementComponent->bOrientRotationToMovement = 1;
			bUseControllerRotationYaw=false;;
			PlayerMovementComponent->AimEnd();
		}

		PlayerAimZoomFunctions->CameraZoomOut(SpringArmComponent, PlayerAimZoomFunctions->CurveTimeline, PlayerAimZoom);
	}
}


void APlayerCharacter::OnCameraMove()
{
	if (CoverData.IsInTransition()) return;
	if (LeftSideView.Block == false && PlayerAimZoom.IsZooming == false && LeftSideView.IsMoving == false)
	{
		LeftSideViewFunctions->OnCameraMove(SpringArmComponent, CameraComponent, LeftSideView, LeftSideViewFunctions->LeftSideViewCurveTimeline);
	}
}


void APlayerCharacter::OnWorldChanged()
{
	TSubclassOf<AChangeWorld> ClassToFind = AChangeWorld::StaticClass();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, ClassToFind, OutActors);
	for (int EveryActor = 0; EveryActor < OutActors.Num(); EveryActor++)
	{
		Cast<AChangeWorld>(OutActors[EveryActor])->Changing();
	}
}


bool APlayerCharacter::StartCover_Internal(FHitResult& CoverHit)
{
	const bool Sup = Super::StartCover_Internal(CoverHit);
	if (!Sup)return false;
	CameraZoomOut();
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
	CoverData.StopCover();
	PlayerMovementComponent->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	return true;
}
