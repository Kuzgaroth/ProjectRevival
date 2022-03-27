// Project Revival. All Rights Reserved

#include "Shell.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	
	FVector Direction = this->GetActorRightVector();
	Direction.Set(Direction.X, Direction.Y, Direction.Z + Rotation * FMath::RandRange(1 - Dispersion, 1 + Dispersion));
	MovementComponent->Velocity = Direction * MovementComponent->InitialSpeed; 
	SetLifeSpan(LifeSeconds);
}
