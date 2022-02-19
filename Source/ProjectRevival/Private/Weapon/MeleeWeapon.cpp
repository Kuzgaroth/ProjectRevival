// Project Revival. All Rights Reserved

#include "Weapon/MeleeWeapon.h"
#include "AssassinEnemy.h"
#include "Weapon/Projectile/BaseProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AMeleeWeapon::AMeleeWeapon()
{
	BeamComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam Component"));
	BeamComp->bAutoActivate = false;
	
	BladeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	BladeCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BladeCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnOverlapBegin);
}

void AMeleeWeapon::AddNewBeam(const FVector Point1, const FVector Point2)
{
	BeamComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamFX, Point1, FRotator::ZeroRotator, true);
	BeamArray.Add(BeamComp);
 	BeamComp->SetBeamSourcePoint(0, Point1, 0);
	BeamComp->SetBeamTargetPoint(0, Point2, 0);
}

void AMeleeWeapon::ToggleCollisionOn()
{
 	BladeCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	const FVector TraceStart = WeaponMesh->GetSocketLocation("TraceStart");
	const FVector TraceEnd = WeaponMesh->GetSocketLocation("TraceEnd");
	if (BeamComp)
	{
		AddNewBeam(TraceStart, TraceEnd);
	}
}

void AMeleeWeapon::ToggleCollisionOff() const
{
	BladeCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		bIsHitDone = true;
		UE_LOG(LogPRAbilitySystemBase, Error, TEXT("Damage done yaaay"));
	}
	UE_LOG(LogPRAbilitySystemBase, Warning, TEXT("OnOverlapBegin"));
	BladeCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}