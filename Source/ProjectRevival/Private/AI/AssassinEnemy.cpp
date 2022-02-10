// Project Revival. All Rights Reserved

// ReSharper disable CppMemberFunctionMayBeConst

#include "AI/AssassinEnemy.h"
#include "Components/WidgetComponent.h"
#include "Components/WeaponComponent.h"

void AAssassinEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(this->InputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward",this,&AAssassinEnemy::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AAssassinEnemy::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this,&AAssassinEnemy::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround",this,&AAssassinEnemy::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed,this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Pressed,this, &AAssassinEnemy::StartRun);
	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Released,this, &AAssassinEnemy::StopRun);
	PlayerInputComponent->BindAction("Fire",EInputEvent::IE_Pressed,WeaponComponent, &UWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire",EInputEvent::IE_Released,WeaponComponent, &UWeaponComponent::StopFire);
	
	AbilitySystemComponent->BindAbilityActivationToInputComponent(this->InputComponent,
	FGameplayAbilityInputBinds(FString("ConfirmTarget"),
	FString("CancelTarget"), FString("EGASInputActions")));
}

void AAssassinEnemy::MoveForward(float Amount)
{
	IsMovingForward = Amount>0;
	if ( (this->GetController() != nullptr) && (Amount != 0.0f))
	{
		const FRotator Rotation = this->GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		this->AddMovementInput(Direction, Amount);
	}
}

void AAssassinEnemy::MoveRight(float Amount)
{
	if ( (this->GetController() != nullptr) && (Amount != 0.0f))
	{
		const FRotator Rotation = this->GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		this->AddMovementInput(Direction, Amount);
	}
}

void AAssassinEnemy::StartRun()
{
	bWantsToRun=true;
}

void AAssassinEnemy::StopRun()
{
	bWantsToRun=false;
}

bool AAssassinEnemy::IsRunning() const
{
	return bWantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

void AAssassinEnemy::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);
}

void AAssassinEnemy::SetMovementMode(EMovementMode Mode)
{
	AssassinMovementComponent->SetMovementMode(Mode);
}

void AAssassinEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent = Cast<UPRAbilitySystemComponent>(this->GetAbilitySystemComponent());
	//AICon = Cast<ASoldierAIController>(GetController());
	//if (AICon)
	//{
	//	BBComp = AICon->GetBlackboardComponent();
	//}
	//UpdateHStateBlackboardKey(3);
}

bool AAssassinEnemy::IsAttacking() const
{
	return bIsAttacking;
}