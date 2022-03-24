// Project Revival. All Rights Reserved

#include "Magazine.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY(LogCustom);

AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	MeshComponent->SetVisibility(true);
	RootComponent = MeshComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->bSimulationEnabled = false;
	MovementComponent->InitialSpeed = 0.f;
}

void AMagazine::AttachMagazine(USceneComponent* ParentMesh, const FName& SocketName)
{
	if (!ParentMesh || !RootComponent)
	{
		UE_LOG(LogCustom,Error,TEXT("AMagazine::AttachMagazine"));
		return;
	}
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	RootComponent->AttachToComponent(ParentMesh, AttachmentTransformRules, SocketName);
}

void AMagazine::DetachMagazine()
{
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	MovementComponent->bSimulationEnabled = true;
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
