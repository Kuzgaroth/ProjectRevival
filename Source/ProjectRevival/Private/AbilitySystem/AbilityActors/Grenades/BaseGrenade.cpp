// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityActors/Grenades/BaseGrenade.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ABaseGrenade::ABaseGrenade()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.TickInterval = 0.01;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CollisionComponent->InitCapsuleSize(5.0f, 7.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	CollisionComponent->SetupAttachment(RootComponent);
	
	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>("GrenadeMesh");
	GrenadeMesh->SetSimulatePhysics(true);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->ProjectileGravityScale = GravityScale;

	RootComponent = GrenadeMesh;
	MovementComponent->bAutoActivate = false;
}

void ABaseGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseGrenade::BeginPlay()
{
	Super::BeginPlay();
	check(GrenadeMesh);
	check(MovementComponent);
	check(CollisionComponent);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseGrenade::OnProjectileHit);
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
}

void ABaseGrenade::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Was used BASE implementation"));
	GetWorldTimerManager().SetTimer(GrenadeActionHandler, this, &ABaseGrenade::GrenadeAction, ActionDelay, false);
}

void ABaseGrenade::UnbindOverlapEvent()
{
	CollisionComponent->OnComponentHit.RemoveDynamic(this, &ABaseGrenade::OnProjectileHit);
}

void ABaseGrenade::GrenadeAction()
{
	SpawnActionEffect();
	SelfDestruction();
}

void ABaseGrenade::SelfDestruction()
{
	GetWorldTimerManager().ClearTimer(GrenadeActionHandler);
	this->Destroy();
}

//Used by other classes when spawning instances of that one. Sets speed for movement component
void ABaseGrenade::SetInitialSpeed(float Speed)
{
	MovementComponent->InitialSpeed = Speed;
	MovementComponent->Activate();
}

void ABaseGrenade::SpawnActionEffect()
{
	if (bUseNiagara)
	{
		if (NiagaraEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, GetActorLocation(), GetActorRotation());
			//Should be used when we need an effect to follow the grenade position. For example, when making sonar grenades. But most of grenades would use AtLocation
			/*UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraEffect, GrenadeMesh, FName(GrenadeMesh->GetName()),
				FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,true);*/
		}
	}
	else
	{
		if(CascadeEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CascadeEffect, GetActorLocation());
			//UGameplayStatics::SpawnEmitterAttached(CascadeEffect, GrenadeMesh);
		}
	}
}
