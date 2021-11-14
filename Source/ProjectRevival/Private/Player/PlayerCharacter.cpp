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
#include "Components/BaseCharacterMovementComponent.h"
#include "ProjectRevival/ProjectRevival.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	ToIgnore.Add(this);

	TraceChannelProvided = ECollisionChannel::ECC_GameTraceChannel2;

	SphereDetectingHighlightables = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Highilght Detector Component"));
	SphereDetectingHighlightables->InitSphereRadius(HighlightRadius);
	SphereDetectingHighlightables->SetCollisionProfileName(TEXT("TriggerHighlighter"));
	SphereDetectingHighlightables->SetupAttachment(RootComponent);
	
	SphereDetectingHighlightables->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBeginForHighlight);
	SphereDetectingHighlightables->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEndForHighlight); 
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
}

void APlayerCharacter::Tick(float DeltaTime)
{
	// if (IsHighlighting == true)
	// {
	// 
	// 	
	// }
	//Super::Tick(DeltaTime);
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

void APlayerCharacter::HighlightAbility()
{
	if (SphereDetectingHighlightables->GetScaledSphereRadius() != HighlightRadius)
	{
		SphereDetectingHighlightables->SetSphereRadius(HighlightRadius);
	}
	TArray<FHitResult> OutHits;
	FVector ActorLocation = GetActorLocation();
	
	bool IsHitKismetByObj = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), ActorLocation, ActorLocation, HighlightRadius,
		ObjectTypesToHighlight, true, ToIgnore, EDrawDebugTrace::None, OutHits, true);
	
	if (IsHighlighting == false)
	{
		if(IsHitKismetByObj)
		{
			for (FHitResult& Hit : OutHits)
			{
				Hit.GetComponent()->SetRenderCustomDepth(true);
			}
		}
		IsHighlighting = true;
	} else if (IsHighlighting == true)
	{
		if(IsHitKismetByObj)
		{
			for (FHitResult& Hit : OutHits)
			{
				Hit.GetComponent()->SetRenderCustomDepth(false);
			}
		}
		IsHighlighting = false;
	}
	
}

void APlayerCharacter::OnOverlapBeginForHighlight(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsHighlighting == true)
	{
		if (OtherActor && (OtherActor != this) && OtherComp)
		{
			if (Cast<ACharacter>(OtherActor))
			{
				ACharacter* CharacterTmp = Cast<ACharacter>(OtherActor);
				CharacterTmp->GetMesh()->SetRenderCustomDepth(true);
			} 
			// else
			// {
			// 	OtherComp->SetRenderCustomDepth(true);
			// }
		}
	}
}

void APlayerCharacter::OnOverlapEndForHighlight(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsHighlighting == true)
	{
		if (OtherActor && (OtherActor != this) && OtherComp)
		{
			if (Cast<ACharacter>(OtherActor))
			{
				ACharacter* CharacterTmp = Cast<ACharacter>(OtherActor);
				CharacterTmp->GetMesh()->SetRenderCustomDepth(false);
			} 
			// else
			// {
			// 	OtherComp->SetRenderCustomDepth(false);
			// }
		}
	}
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
		const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
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

void APlayerCharacter::Flip()
{
	//добавить в CanFire IfFlipping
	if(GetCharacterMovement()->IsFlying()||GetCharacterMovement()->IsFalling()||WeaponComponent->IsShooting()
		||!WeaponComponent->CanFire()||IsFlipping)
	{
		UE_LOG(LogPlayerCharacter, Warning, TEXT("Flip failed"));
	}
	else
	{	
     	FVector Forward;
		float Length;
		GetVelocity().ToDirectionAndLength(Forward, Length);
     	Forward.Z = 0;
		const FVector TempVelocity = GetCharacterMovement()->Velocity;
     	const TSharedPtr<FRootMotionSource_ConstantForce> ConstantForce = MakeShared<FRootMotionSource_ConstantForce>();
     	ConstantForce->InstanceName = "Flip";
     	ConstantForce->AccumulateMode = ERootMotionAccumulateMode::Override;
     	ConstantForce->Priority = 5;
     	ConstantForce->Force = Forward * FlipStrength;
     	ConstantForce->Duration = FlipTime;
     	// ConstantForce->StrengthOverTime = nullptr;
		// change strength value during movement:
		ConstantForce->StrengthOverTime = FlipCurve;
     	ConstantForce->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::SetVelocity;
		ConstantForce->FinishVelocityParams.SetVelocity = TempVelocity;
		//SpringArmComponent->bUsePawnControlRotation = false;
     	bUseControllerRotationYaw = false; 
		IsFlipping = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->ApplyRootMotionSource(ConstantForce);
		GetWorld()->GetTimerManager().SetTimer(THandle, this, &APlayerCharacter::StopFlip, FlipTime, false);
	}
}

void APlayerCharacter::StopFlip()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	SpringArmComponent->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;
	IsFlipping = false;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	UE_LOG(LogPlayerCharacter, Verbose, TEXT("Flip was successful"));
}

void APlayerCharacter::On_Camera_Move()
 {
 	FTimerHandle TimerCameraMove;
 	FTimerHandle TimerCameraStop;
 	FTimerHandle TimerCameraBlock;
 	if (Block == false)
 	{
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
 				SpringArmComponent->SocketOffset.Y = CameraComponent->GetRelativeLocation().Y + 150.f;
 				CamPos = false;
 			}
 			else {CamPos = true;}
 		}
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
 
 void APlayerCharacter::Camera_Block()
 {
 	Block = false;
 }
