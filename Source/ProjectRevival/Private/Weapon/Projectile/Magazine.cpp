// Project Revival. All Rights Reserved

#include "Magazine.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMagazine::AMagazine()
{
	GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Green, __FUNCTION__);
	PrimaryActorTick.bCanEverTick = false;
			
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalComponent");
	RootComponent = SkeletalMeshComponent;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->InitSphereRadius(30.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = 0.0f;
}

void AMagazine::AttachMagazine(USceneComponent* ParentMesh, const FName& SocketName)
{
	GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Green, __FUNCTION__);
	if (!ParentMesh) return;
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	RootComponent->AttachToComponent(ParentMesh, AttachmentTransformRules, SocketName);
}

void AMagazine::DetachMagazine(USceneComponent* ParentMesh, const FName& SocketName)
{
	GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Green, __FUNCTION__);
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetLifeSpan(LifeSeconds);
}

void AMagazine::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Green, __FUNCTION__);
	Super::BeginPlay();

	check(MovementComponent);
	check(CollisionComponent);
	check(SkeletalMeshComponent);

	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
}
