// Project Revival. All Rights Reserved

#include "Magazine.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY(LogCustom);

AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->bSimulationEnabled = false;
	MovementComponent->InitialSpeed = 10.f;
}

void AMagazine::DetachMagazine()
{
	this->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
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
