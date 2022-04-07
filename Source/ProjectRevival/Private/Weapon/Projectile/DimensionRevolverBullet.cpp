// Project Revival. All Rights Reserved


#include "Weapon/Projectile/DimensionRevolverBullet.h"

#include "BaseCharacter.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "WeaponFXComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

FComponentHitSignature ADimensionRevolverBullet::GetHitDelegate() const
{
	return CollisionComponent->OnComponentHit;
}

void ADimensionRevolverBullet::BeginPlay()
{
	Super::BeginPlay();
	check(MovementComponent)
	check(WeaponFXComponent)
	
	MovementComponent->Velocity=ShotDirection*MovementComponent->InitialSpeed;
	CollisionComponent->OnComponentHit.AddDynamic(this, &ADimensionRevolverBullet::OnProjectileHit);
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	SetLifeSpan(LifeSeconds);
}

void ADimensionRevolverBullet::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GLog->Log(GetOwner()->GetName());
	if (!GetWorld()) return;
	MovementComponent->StopMovementImmediately();

	if(OtherActor)
	{
		auto Bot=Cast<ABaseCharacter>(OtherActor);
		if(Bot)
		{
			UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation() , DamageRadius,
			UDamageType::StaticClass(), {}, this, GetController(), bDoFullDamage);
			auto Player=Cast<APlayerCharacter>(GetOwner());
			auto PlayerHealthComponent=Player->GetHealthComponent();
			PlayerHealthComponent->TryToAddHealth(30.0f);
		}
	}
	//DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
	WeaponFXComponent->PlayImpactFX(Hit);
	Destroy();

}

void ADimensionRevolverBullet::LifeSpanExpired()
{
	FHitResult result;
	CollisionComponent->OnComponentHit.Broadcast(nullptr,nullptr,nullptr,FVector::ZeroVector,result);
	Super::LifeSpanExpired();
}



