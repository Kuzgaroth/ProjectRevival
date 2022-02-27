// Project Revival. All Rights Reserved

// ReSharper disable CppMemberFunctionMayBeConst

#include "AI/AssassinEnemy.h"
#include "Components/WidgetComponent.h"
#include "Components/WeaponComponent.h"

bool AAssassinEnemy::IsRunning() const
{
	return IsMovingForward && !GetVelocity().IsZero();
}

void AAssassinEnemy::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);
}

void AAssassinEnemy::SetMovementMode(EMovementMode Mode)
{
	//AssassinMovementComponent->SetMovementMode(Mode);
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