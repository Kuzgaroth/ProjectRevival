// Project Revival. All Rights Reserved


#include "Weapon/SoldierRifleWeapon.h"
#include "BaseCharacter.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

void ASoldierRifleWeapon::StartFire()
{
	InitFX();
	UE_LOG(LogPRAISoldier, Log, TEXT("Rifle: StartFire() was called"))
	CurrentBurstRow = BurstClipsNumber;
	OneRowTime = BurstBulletsDelay * BurstBulletsNumber + BurstClipsDelay;
	GetWorld()->GetTimerManager().SetTimer(ClipsTimerHandle, this, &ASoldierRifleWeapon::ShootRowInternal, OneRowTime, true);
}

void ASoldierRifleWeapon::ShootRowInternal()
{
	--CurrentBurstRow;
	if (CurrentBurstRow < 0)
	{
		StopFire();
	}
	CurrentBurstShot = BurstBulletsNumber;
	SetFXActive(true);
	GetWorld()->GetTimerManager().SetTimer(BulletsTimerHandle, this, &ASoldierRifleWeapon::MakeShotInternal, BurstBulletsDelay, true);
}

void ASoldierRifleWeapon::MakeShotInternal()
{
	UE_LOG(LogTemp, Warning, TEXT("Current Burst Shot: %d"), CurrentBurstShot);
	MakeShot();
	SetFXActive(true);
	--CurrentBurstShot;
	if (CurrentBurstShot <= 0)
	{
		StopFireBullet();
	}
}

void ASoldierRifleWeapon::StopFire()
{
	UE_LOG(LogTemp, Log, TEXT("Rifle has stopped fire. It's Owner is: %s"), *GetOwner()->GetName());
	if (GetWorld()->GetTimerManager().IsTimerActive(BulletsTimerHandle)) {GetWorld()->GetTimerManager().ClearTimer(BulletsTimerHandle);}
	GetWorld()->GetTimerManager().ClearTimer(ClipsTimerHandle);
	StoppedFireInWeaponDelegate.Broadcast();
	SetFXActive(false);
}

void ASoldierRifleWeapon::StopFireExternal()
{
	UE_LOG(LogTemp, Log, TEXT("Rifle has stopped fire. It's Owner is: %s"), *GetOwner()->GetName());
	if (GetWorld()->GetTimerManager().IsTimerActive(BulletsTimerHandle)) {GetWorld()->GetTimerManager().ClearTimer(BulletsTimerHandle);}
	GetWorld()->GetTimerManager().ClearTimer(ClipsTimerHandle);
	SetFXActive(false);
}

void ASoldierRifleWeapon::StopFireBullet()
{
	SetFXActive(false);
	GetWorld()->GetTimerManager().ClearTimer(BulletsTimerHandle);
}

void ASoldierRifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = BurstBulletsDelay;
}