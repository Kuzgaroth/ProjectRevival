// Project Revival. All Rights Reserved

#include "Shell.h"
#include "BaseCharacter.h"
#include "WeaponComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AShell::AShell()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = Speed;
}

void AShell::BeginPlay()
{
	Super::BeginPlay();
	check(MovementComponent);
	check(MeshComponent);
	FVector Direction = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))->GetActorRightVector();
	const FVector DeltaVector =  (-1) * Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))->GetActorForwardVector()
		* FMath::RandRange(RotationXY - Dispersion, RotationXY + Dispersion);
	Direction = Direction + DeltaVector;
	Direction.Z += FMath::RandRange(-Dispersion, Dispersion) + RotationZ;
	MovementComponent->Velocity = MovementComponent->InitialSpeed * Direction; 
	SetLifeSpan(LifeSeconds);
}
