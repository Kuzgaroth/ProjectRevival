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
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	TraceChannelProvided = ECollisionChannel::ECC_GameTraceChannel2;
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward",this,&APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this,&APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround",this,&APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed,this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Pressed,this, &APlayerCharacter::StartRun);
	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Released,this, &APlayerCharacter::StopRun);
	//PlayerInputComponent->BindAction("Flip",EInputEvent::IE_Pressed,this, &APlayerCharacter::Flip);
	//PlayerInputComponent->BindAction("Highlight",EInputEvent::IE_Pressed,this, &APlayerCharacter::HighlightAbility);
	PlayerInputComponent->BindAction("Fire",EInputEvent::IE_Pressed,WeaponComponent, &UWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire",EInputEvent::IE_Released,WeaponComponent, &UWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon",EInputEvent::IE_Pressed,WeaponComponent, &UWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload",EInputEvent::IE_Pressed,WeaponComponent, &UWeaponComponent::Reload);
	PlayerInputComponent->BindAction("Left_Camera_View", EInputEvent::IE_Pressed,this, &APlayerCharacter::On_Camera_Move);
	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent,
	FGameplayAbilityInputBinds(FString("ConfirmTarget"),
	FString("CancelTarget"), FString("EGASInputActions")));
	InputComponent->BindAction("Zoom", IE_Pressed, this, &APlayerCharacter::CameraZoomIn);
	InputComponent->BindAction("Zoom", IE_Released, this, &APlayerCharacter::CameraZoomOut);
}

void APlayerCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount>0.f;
	AddMovementInput(GetActorForwardVector(),Amount);
}

void APlayerCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(),Amount);
}

void APlayerCharacter::StartRun()
{
	bWantsToRun=true;
}

void APlayerCharacter::StopRun()
{
	bWantsToRun=false;
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

bool APlayerCharacter::IsRunning() const
{
	return bWantsToRun && IsMovingForward && !GetVelocity().IsZero();
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

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnCameraCollisionEndOverlap);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline(DeltaTime);
}


void APlayerCharacter::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(PlayerAimZoom.StartLoc, PlayerAimZoom.EndLoc, Value);
	SpringArmComponent->SocketOffset = NewLocation;
}


void APlayerCharacter::On_Camera_Move()
{
	FTimerHandle TimerCameraMove;
	FTimerHandle TimerCameraStop;
	FTimerHandle TimerCameraBlock;
	if (Block == false && PlayerAimZoom.IsZooming == false)
	{
		float StartPos = SpringArmComponent->SocketOffset.Y;
		InterpSpeed = (SpringArmComponent->SocketOffset.Y + tan(CameraComponent->GetRelativeRotation().Yaw * PI / 180) * SpringArmComponent->TargetArmLength) * 2.f / 50.f;
		if (IsMoving == false)
		{
			IsMoving = true;
			Block = true;
			GetWorld()->GetTimerManager().SetTimer(TimerCameraMove, this, &APlayerCharacter::Camera_Moving, 0.01f, true);
			GetWorld()->GetTimerManager().SetTimer(TimerCameraStop, this, &APlayerCharacter::Camera_Stop, 0.5f, false);
		}
		else
		{
			IsMoving = false;
			Block = true;
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
			GetWorld()->GetTimerManager().SetTimer(TimerCameraBlock, this, &APlayerCharacter::Camera_Block, 1.f, false);
			if (CamPos == true)
			{
				SpringArmComponent->SocketOffset.Y = StartPos;
				CamPos = false;
			}
			else {CamPos = true;}
			PlayerAimZoom.StartStartPos = SpringArmComponent->SocketOffset;
		}
	}
}

void APlayerCharacter::Camera_Block()
{
	Block = false;
}

void APlayerCharacter::CameraZoomIn()
{
	if (IsMoving == false)
	{
		if (PlayerAimZoom.StartStartPos == FVector(0.0, 0.0, 0.0)) PlayerAimZoom.StartStartPos = SpringArmComponent->SocketOffset;
		SpringArmComponent->SocketOffset = PlayerAimZoom.StartStartPos;
		FOnTimelineVector TimelineProgress;
		FOnTimelineFloat TimelineFieldOfView;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		TimelineFieldOfView.BindUFunction(this, FName("TimelineFieldOfView"));
		CurveTimeline.AddInterpVector(PlayerAimZoom.CurveVector, TimelineProgress);
		CurveTimeline.AddInterpFloat(PlayerAimZoom.CurveFloat, TimelineFieldOfView);

		PlayerAimZoom.StartLoc = SpringArmComponent->SocketOffset;
		PlayerAimZoom.EndLoc = FVector(SpringArmComponent->SocketOffset.X + PlayerAimZoom.Offset.X, SpringArmComponent->SocketOffset.Y, SpringArmComponent->SocketOffset.Z + PlayerAimZoom.Offset.Z);
		if (CamPos == false) PlayerAimZoom.EndLoc.Y -= PlayerAimZoom.Offset.Y; else PlayerAimZoom.EndLoc.Y += PlayerAimZoom.Offset.Y / 2.0;
		PlayerAimZoom.CurrentFieldOfView = PlayerAimZoom.FieldOfView;

		PlayerAimZoom.IsZooming = true;
		CurveTimeline.PlayFromStart();
	}
}

void APlayerCharacter::CameraZoomOut()
{
	if (IsMoving == false && PlayerAimZoom.IsZooming == true)
	{
		FOnTimelineVector TimelineProgress;
		FOnTimelineFloat TimelineFieldOfView;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		TimelineFieldOfView.BindUFunction(this, FName("TimelineFieldOfView"));
		CurveTimeline.AddInterpVector(PlayerAimZoom.CurveVector, TimelineProgress);
		CurveTimeline.AddInterpFloat(PlayerAimZoom.CurveFloat, TimelineFieldOfView);
	
		PlayerAimZoom.EndLoc = PlayerAimZoom.StartLoc;
		PlayerAimZoom.StartLoc = SpringArmComponent->SocketOffset;
		PlayerAimZoom.CurrentFieldOfView = 90.0;

		PlayerAimZoom.IsZooming = false;
		CurveTimeline.PlayFromStart();
	}
}
 
 void APlayerCharacter::Camera_Moving()
 {
 	SpringArmComponent->SocketOffset.Y = FMath::FInterpTo(SpringArmComponent->SocketOffset.Y, SpringArmComponent->SocketOffset.Y - InterpSpeed, 1.f, InterpSpeed);
 }
 
 void APlayerCharacter::Camera_Stop()
 {
 	Block = false;
 	On_Camera_Move();
 }
