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
	MeshComponent->OnComponentHit.AddDynamic(this,&AShell::PlaySoundAtHit);
}

void AShell::PlaySoundAtHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const auto PhysMaterial = Hit.PhysMaterial.Get();
	if (SoundMap.Contains(PhysMaterial))
	{
		UGameplayStatics::SpawnSoundAttached(SoundMap[PhysMaterial],GetRootComponent());
	}
	else
	{
		if (DefaultHitSound)
		{
			UGameplayStatics::SpawnSoundAttached(DefaultHitSound,GetRootComponent());
		}
	}
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
