// Project Revival. All Rights Reserved

#include "Magazine.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = false;
	
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetSimulatePhysics(false);
	
	RootComponent = MeshComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->bSimulationEnabled = false;
	MovementComponent->InitialSpeed = 10.f;
	MovementComponent->MaxSpeed = 600.f;
}

void AMagazine::DetachMagazine()
{
	const FDetachmentTransformRules TransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, true);
	this->DetachFromActor(TransformRules);
	
	MovementComponent->bSimulationEnabled = true;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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
