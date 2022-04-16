// Project Revival. All Rights Reserved


#include "Weapon/DimensionRevolver.h"

#include "BaseProjectile.h"
#include "DimensionRevolverBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ADimensionRevolver::MakeShot()
{
	if (!GetWorld()) return;

	if (IsAmmoEmpty())
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
		return;
	}
	FVector TraceStart;
	FVector TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult,TraceStart, TraceEnd);

	FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint-GetMuzzleWorldLocation()).GetSafeNormal();
	
	const FTransform SpawnTransform(FRotator::ZeroRotator, WeaponMesh->GetSocketTransform(MuzzelSocketName).GetLocation());
	auto Projectile = GetWorld()->SpawnActorDeferred<ADimensionRevolverBullet>(ProjectileClass, SpawnTransform);
	
	if (Projectile)
	{
		Projectile->SetHealPercentage(HealPercent);
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

void ADimensionRevolver::SetHealPercentToBullet(float Percent)
{
	HealPercent=Percent;
}
