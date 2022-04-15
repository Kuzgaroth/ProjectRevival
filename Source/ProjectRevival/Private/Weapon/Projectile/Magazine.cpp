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
}

void AMagazine::DetachMagazine()
{
	const FDetachmentTransformRules TransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, true);
	this->DetachFromActor(TransformRules);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetSimulatePhysics(true);
	this->MeshComponent->AddImpulse((-this->GetActorUpVector() + this->GetActorRightVector()) * InitialFallingSpeed);
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
