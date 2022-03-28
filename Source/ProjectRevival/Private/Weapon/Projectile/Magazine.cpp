// Project Revival. All Rights Reserved

#include "Magazine.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY(LogCustom);

AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scale.X = 1.0; Scale.Y = 1.0; Scale.Z = 1.0;
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetWorldScale3D(Scale);
	
	RootComponent = MeshComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->bSimulationEnabled = false;
	MovementComponent->InitialSpeed = 10.f;
}

void AMagazine::DetachMagazine()
{
	
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	//SimulatePhysics don't work with ProjectileMovementComponent, but looks pretty cool:
	//MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//MeshComponent->SetSimulatePhysics(true);
	
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetWorldScale3D(Scale);
	
	MovementComponent->bSimulationEnabled = true;
	MovementComponent->Velocity = -this->GetActorUpVector() * MovementComponent->InitialSpeed; 
	SetLifeSpan(LifeSeconds);
}

USkeletalMeshComponent* AMagazine::GetMeshComponent()
{
	if(MeshComponent) return MeshComponent;
	else return nullptr;
}

void AMagazine::BeginPlay()
{
	Super::BeginPlay();
	check(MeshComponent);
}
