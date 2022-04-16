// Project Revival. All Rights Reserved


#include "Weapon/Projectile/DimensionRevolverBullet.h"

#include "BaseCharacter.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "WeaponFXComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


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
		const auto Bot=Cast<ABaseCharacter>(OtherActor);
		if(Bot)
		{
			const auto Damage= Bot->GetHealthComponent()->GetMaxHeatlh();
			UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation() , DamageRadius,
			UDamageType::StaticClass(), {}, this, GetController(), bDoFullDamage);
			const auto Player=Cast<APlayerCharacter>(GetOwner());
			const auto PlayerHealthComponent=Player->GetHealthComponent();
			PlayerHealthComponent->TryToAddHealthPercentage(HealHealthPercent);
		}
	}
	//DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
	WeaponFXComponent->PlayImpactFX(Hit);
	Destroy();

}




