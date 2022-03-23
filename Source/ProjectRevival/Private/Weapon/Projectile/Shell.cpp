// Project Revival. All Rights Reserved


#include "Shell.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY(LogCustom);

// Sets default values
AShell::AShell()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = 500.0f;
}

UStaticMeshComponent* AShell::GetMesh() const
{
	if(MeshComponent) return MeshComponent;
	else return nullptr;
}

// Called when the game starts or when spawned
void AShell::BeginPlay()
{
	UE_LOG(LogCustom, Error, TEXT("Shell BeginPlay"));
	Super::BeginPlay();
	
	check(MovementComponent);
	//MovementComponent->Velocity = MovementDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	SetLifeSpan(LifeSeconds);
}
