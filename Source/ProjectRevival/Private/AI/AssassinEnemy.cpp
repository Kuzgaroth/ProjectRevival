// Project Revival. All Rights Reserved

// ReSharper disable CppMemberFunctionMayBeConst

#include "AI/AssassinEnemy.h"
#include "Components/WidgetComponent.h"

void AAssassinEnemy::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);
	UE_LOG(LogPRAICharacter, Warning, TEXT("Started all shit"));
}

bool AAssassinEnemy::IsAttacking() const
{
	return bIsAttacking;
}
