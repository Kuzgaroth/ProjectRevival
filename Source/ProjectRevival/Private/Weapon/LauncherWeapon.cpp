// Project Revival. All Rights Reserved


#include "Weapon/LauncherWeapon.h"
#include "Weapon/Projectile/BaseProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ALauncherWeapon::MakeShot()
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

void ALauncherWeapon::StartFire()
{
	MakeShot();
	
}

void ALauncherWeapon::StopFire()
{
	
}