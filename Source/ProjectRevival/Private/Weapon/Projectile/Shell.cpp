// Project Revival. All Rights Reserved

#include "Shell.h"
#include "BaseCharacter.h"
#include "WeaponComponent.h"
#include "Kismet/GameplayStatics.h"

AShell::AShell()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;
}

void AShell::BeginPlay()
{
	Super::BeginPlay();
	check(MeshComponent);
	
	FVector Direction = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))->GetActorRightVector();
	const FVector DeltaVector =  (-1) * Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))->GetActorForwardVector()
		* FMath::RandRange(RotationXY - Dispersion, RotationXY + Dispersion);
	Direction = Direction + DeltaVector;
	Direction.Z += FMath::RandRange(-Dispersion, Dispersion) + RotationZ;

	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetPhysicsLinearVelocity(Direction * Speed);
	
	SetLifeSpan(LifeSeconds);
}
