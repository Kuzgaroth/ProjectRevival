// Project Revival. All Rights Reserved

#include "Weapon/MeleeWeapon.h"
#include "Weapon/Projectile/BaseProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AMeleeWeapon::AMeleeWeapon()
{
	BeamComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam Component"));
	BeamComp->bAutoActivate = false;
	
	BladeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
}

void AMeleeWeapon::AddNewBeam(FVector Point1, FVector Point2)
{
     
	BeamComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamFX, Point1, FRotator::ZeroRotator, true);
	BeamArray.Add(BeamComp);
 	BeamComp->SetBeamSourcePoint(0, Point1, 0);
	//BeamComp->SetBeamTargetPoint(0, Point2, 0);
}

void AMeleeWeapon::StartFire()
{
}

void AMeleeWeapon::StopFire()
{
}

/*
void AMeleeWeapon::MakeShot()
{
	if (!GetWorld()) return;

	
	FVector TraceStart;
	FVector TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult,TraceStart, TraceEnd);

	FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint-GetMuzzleWorldLocation()).GetSafeNormal();
	
	const FTransform SpawnTransform(FRotator::ZeroRotator, WeaponMesh->GetSocketTransform(MuzzelSocketName).GetLocation());
	auto Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass, SpawnTransform);
	
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}
	DecreaseAmmo();
	if (bUseNiagaraMuzzleEffect == true)
	{
		SpawnMuzzleFXNiagara();
	}
	else
	{
		SpawnMuzzleFXCascade();
	}

	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzelSocketName);
}
*/