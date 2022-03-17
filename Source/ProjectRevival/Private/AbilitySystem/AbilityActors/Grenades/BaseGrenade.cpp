// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityActors/Grenades/BaseGrenade.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ABaseGrenade::ABaseGrenade()
{
	PrimaryActorTick.bCanEverTick = false;
	// PrimaryActorTick.TickInterval = 0.01;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CollisionComponent->bReturnMaterialOnMove = true;
	RootComponent = CollisionComponent;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>("GrenadeMesh");
	GrenadeMesh->SetSimulatePhysics(true);
	GrenadeMesh->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->bAutoActivate = false;
}

void ABaseGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseGrenade::BeginPlay()
{
	Super::BeginPlay();
	if (bShouldBlow)
	{
		// UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Current InitialSpeed is %f"), MovementComponent->InitialSpeed);
		// UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Current Owner of grenade is %s"), *GetOwner()->GetName());
		// UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("Current Velocity of grenade is %s"), *MovementComponent->Velocity.ToString());
		CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseGrenade::OnProjectileHit);
		CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
		MovementComponent->Activate();
	}
}

void ABaseGrenade::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsFirstHit)
	{
		bIsFirstHit = false;
		// UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Was used BASE implementation"));
		GetWorldTimerManager().SetTimer(GrenadeActionHandler, this, &ABaseGrenade::GrenadeAction, ActionDelay, false);
	}
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
	UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Set initial speed function was triggered"));
	UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Speed passed to grenade: %f"), Speed);
	MovementComponent->InitialSpeed = Speed;
	UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Speed Initial in Set: %f"), MovementComponent->InitialSpeed);
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
