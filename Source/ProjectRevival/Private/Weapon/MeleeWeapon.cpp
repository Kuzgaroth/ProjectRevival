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
	BladeCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeWeapon::AddNewBeam(FVector Point1, FVector Point2)
{
     
	BeamComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamFX, Point1, FRotator::ZeroRotator, true);
	BeamArray.Add(BeamComp);
 	BeamComp->SetBeamSourcePoint(0, Point1, 0);
	BeamComp->SetBeamTargetPoint(0, Point2, 0);
}

void AMeleeWeapon::StartFire()
{
	MakeShot();
}

void AMeleeWeapon::StopFire()
{
}

void AMeleeWeapon::MakeShot()
{
	BladeCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnOverlapBegin);
	BladeCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FVector TraceStart = WeaponMesh->GetSocketLocation("TraceStart");
	FVector TraceEnd = WeaponMesh->GetSocketLocation("TraceEnd");
	if (BeamComp)
	{
		AddNewBeam(TraceStart, TraceEnd);
	}
}

void AMeleeWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UE_LOG(LogCore, Warning, TEXT("Hit done"));
	}
	BladeCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}