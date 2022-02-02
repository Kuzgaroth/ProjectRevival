// Project Revival. All Rights Reserved

#include "Weapon/MeleeWeapon.h"
#include "Weapon/Projectile/BaseProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AMeleeWeapon::AMeleeWeapon()
{
	//BeamComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BeamComp"));
	//BeamComp->SetupAttachment(SpringArmComp);
	//BeamComp->bAutoActivate = false;
}

void AMeleeWeapon::AddNewBeam(FVector Point1, FVector Point2)
{
     
	BeamComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamFX, Point1, FRotator::ZeroRotator, true);
	BeamArray.Add(BeamComp);
 
	//BeamComp->SetBeamSourcePoint(0, Point1, 0);
	//BeamComp->SetBeamTargetPoint(0, Point2, 0);
}
 

void AMeleeWeapon::MakeShot()
{
	if (!GetWorld()) return;
	
	const FTransform SpawnTransform(FRotator::ZeroRotator, WeaponMesh->GetSocketTransform(MuzzelSocketName).GetLocation());
		
	if(HitSound)
	{
		UGameplayStatics::SpawnSoundAttached(HitSound, WeaponMesh, MuzzelSocketName);
	}
}