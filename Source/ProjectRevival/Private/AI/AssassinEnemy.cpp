// Project Revival. All Rights Reserved

// ReSharper disable CppMemberFunctionMayBeConst

#include "AI/AssassinEnemy.h"
#include "Components/WidgetComponent.h"
#include "Abilities/GameplayAbilityTypes.h"

void AAssassinEnemy::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);
}

void AAssassinEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ABaseCharacter::SetupPlayerInputComponent(PlayerInputComponent);
	//PlayerInputComponent->BindAxis("MoveForward",this,&AAssassinEnemy::MoveForward);
	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent,
	FGameplayAbilityInputBinds(FString("ConfirmTarget"),
	FString("CancelTarget"), FString("EGASInputActions")));
}

bool AAssassinEnemy::IsAttacking() const
{
	return bIsAttacking;
}
/*
void AAssassinEnemy::ActivateAbility()
{
	if(IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->AbilityLocalInputPressed();
	}
}

void AAssassinEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AICon = Cast<AAIController>(GetController());
	if (AICon)
	{
		BBComp = AICon->GetBlackboardComponent();
	}
	UE_LOG(LogPRAICharacter, Warning, TEXT("Started all shit"));
	//UpdateHStateBlackboardKey(3);
}*/
void AAssassinEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent = Cast<UPRAbilitySystemComponent>(this->GetAbilitySystemComponent());
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		AddStartupGameplayAbilities();
	}
}
