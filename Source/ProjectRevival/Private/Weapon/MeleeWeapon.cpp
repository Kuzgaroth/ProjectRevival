// Project Revival. All Rights Reserved

#include "Weapon/MeleeWeapon.h"
#include "AssassinEnemy.h"
#include "HealthComponent.h"
#include "Weapon/Projectile/BaseProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AMeleeWeapon::AMeleeWeapon()
{
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	BeamComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam Component"));
	BeamComp->bAutoActivate = false;
	BeamComp->SetupAttachment(RootComponent);
	
	BladeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	BladeCollisionBox->SetupAttachment(RootComponent);
	BladeCollisionBox->SetGenerateOverlapEvents(true);
	BladeCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnOverlapBegin);
	BladeCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	check(BladeCollisionBox);
}

void AMeleeWeapon::AddNewBeam()
{
	const FVector TraceStart = WeaponMesh->GetSocketLocation("TraceStart");
	const FVector TraceEnd = WeaponMesh->GetSocketLocation("TraceEnd");
	if (BeamComp)
	{
		BeamComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamFX, TraceStart, FRotator::ZeroRotator, true);
		BeamArray.Add(BeamComp);
		BeamComp->SetBeamSourcePoint(0, TraceStart, 0);
		BeamComp->SetBeamTargetPoint(0, TraceEnd, 0);
	}
}

void AMeleeWeapon::ToggleCollisionOff() const
{
	BladeCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeWeapon::ToggleCollisionOn() const
{
	BladeCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMeleeWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
	if (OtherActor && Character && (OtherActor != this) && OtherComp && OtherActor != this->GetOwner() && !bIsHitDone)
	{
		Character->TakeDamage(HitDamage,FDamageEvent{},Character->GetController(),this);
		bIsHitDone = true;
	}
}