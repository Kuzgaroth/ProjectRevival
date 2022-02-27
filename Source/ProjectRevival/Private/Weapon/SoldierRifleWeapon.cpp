// Project Revival. All Rights Reserved


#include "Weapon/SoldierRifleWeapon.h"

#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

void ASoldierRifleWeapon::StartFire()
{
	InitFX();
	CurrentBurstShot = BurstClipsNumber * BurstBulletsNumber;
	CurrentBurstRow = BurstClipsNumber;
	OneRowTime = BurstBulletsDelay * BurstBulletsNumber + BurstClipsDelay;
	GetWorld()->GetTimerManager().SetTimer(ClipsTimerHandle, this, &ASoldierRifleWeapon::ShootRowInternal, OneRowTime, true);
}

void ASoldierRifleWeapon::ShootRowInternal()
{
	--CurrentBurstShot;
	if (CurrentBurstShot <= 0)
	{
		Cast<UWeaponComponent>(GetOwner())->StopFire();
	}
	GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ASoldierRifleWeapon::MakeShotInternal, BurstBulletsDelay, true);
	MakeShotInternal();
}

void ASoldierRifleWeapon::StopFire()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(ClipsTimerHandle)) {return;}
	if (GetWorld()->GetTimerManager().IsTimerActive(ShotTimerHandle)) {GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);}
	GetWorld()->GetTimerManager().ClearTimer(ClipsTimerHandle);
	StoppedFireInWeaponDelegate.Broadcast();
	SetFXActive(false);
}

void ASoldierRifleWeapon::MakeShotInternal()
{
	--CurrentBurstShot;
	if (CurrentBurstShot <= 0)
	{
		StopFireInternal();
	}
	MakeShot();
}

void ASoldierRifleWeapon::StopFireInternal()
{
	GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);
}

void ASoldierRifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = BurstBulletsDelay;
}