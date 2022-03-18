// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityActors/Grenades/ExplosiveGrenade.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

AExplosiveGrenade::AExplosiveGrenade()
{
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComponent->Falloff = RIF_Linear;
	RadialForceComponent->Radius = ExplosionRadius;
	RadialForceComponent->ForceStrength = 50;
}

void AExplosiveGrenade::BeginPlay()
{
	Super::BeginPlay();
	// CollisionComponent->OnComponentHit.AddDynamic(this, &AExplosiveGrenade::OnProjectileHit);
}

void AExplosiveGrenade::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsFirstHit)
	{
		bIsFirstHit = false;
		// UE_LOG(LogPRAbilitySystemBase, Log, TEXT("Was used EXPLOSIVE implementation"));
		GetWorldTimerManager().SetTimer(GrenadeActionHandler, this, &AExplosiveGrenade::GrenadeAction, ActionDelay, false);
	}
}

void AExplosiveGrenade::GrenadeAction()
{
	SpawnActionEffect();
	if (!GetWorld()) return;
	MovementComponent->StopMovementImmediately();
	//  TArray<FHitResult> OutHits;
	//  const bool IsHitKismetByObj = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 0, 0.001),
	//  	ExplosionRadius, ObjectTypesToDamage, true, ToBeIgnoredByVisor, EDrawDebugTrace::None, OutHits, true);
	//  UE_LOG(LogTemp, Warning, TEXT("OutHitsOuter Array.Num: %d"), OutHits.Num());
	//  UE_LOG(LogTemp, Warning, TEXT("FirstHitName: %s"), *OutHits.Last().GetActor()->GetName());
	//  for (auto OutHit : OutHits)
	//  {
	//  	TArray<FHitResult> OutHitsLocal;
	//  	UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), GetActorLocation(), OutHit.GetComponent()->GetComponentLocation(),
	//  	ObjectTypesToDamage, true, ToBeIgnoredByVisor, EDrawDebugTrace::None, OutHitsLocal, true);
	//  	UE_LOG(LogTemp, Warning, TEXT("OutHitsLocal Array.Num: %d"), OutHitsLocal.Num());
	//  	if (OutHitsLocal.Num() > 0)
	//  	{
	//  		bool Flag = true;
	//  		for (auto OutHitLocal : OutHitsLocal)
	//  		{
	//  			if (!Cast<ABaseCharacter>(OutHitLocal.GetActor()))
	//  			{
	//  				Flag = false;
	//  			}
	//  		}
	//  		if (Flag)
	//  		{
	//  			Cast<AActor>(OutHitsLocal.Last().GetActor())->ReceiveRadialDamage(ExplosionDamage, Cast<UDamageType>(UDamageType::StaticClass()),
	//  				OutHitsLocal[0].Location, OutHitsLocal[0], GetController(), this);
	//  		}
	//  	}
	//  	OutHitsLocal.Empty();
	// }
	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation() , ExplosionRadius,
		UDamageType::StaticClass(), {}, this, GetController(), false);
	SelfDestruction();
}

void AExplosiveGrenade::SpawnActionEffect()
{
	Super::SpawnActionEffect();
}

void AExplosiveGrenade::SelfDestruction()
{
	Super::SelfDestruction();
}

AController* AExplosiveGrenade::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
