// Project Revival. All Rights Reserved


#include "Weapon/SoldierRifleWeapon.h"
#include "BaseCharacter.h"
#include "NiagaraComponent.h"
#include "SoldierEnemy.h"
#include "Kismet/GameplayStatics.h"

void ASoldierRifleWeapon::StartFire()
{
	InitFX();
	CurrentBurstRow = BurstClipsNumber;
	OneRowTime = BurstBulletsDelay * BurstBulletsNumber + BurstClipsDelay;
	GetWorld()->GetTimerManager().SetTimer(ClipsTimerHandle, this, &ASoldierRifleWeapon::ShootRowInternal, OneRowTime, true);
}

void ASoldierRifleWeapon::ShootRowInternal()
{
	--CurrentBurstRow;
	if (CurrentBurstRow < 0)
	{
		StopFireInternal();
	}
	CurrentBurstShot = BurstBulletsNumber;
	SetFXActive(true);
	GetWorld()->GetTimerManager().SetTimer(BulletsTimerHandle, this, &ASoldierRifleWeapon::MakeShotInternal, BurstBulletsDelay, true);
}

void ASoldierRifleWeapon::MakeShotInternal()
{
	UE_LOG(LogTemp, Warning, TEXT("Current Burst Shot: %d"), CurrentBurstShot);
	MakeShot();
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
	CurrentBurstShot = BurstBulletsNumber;
	SetFXActive(false);
	StoppedFireInWeaponDelegate.Broadcast();
}

void ASoldierRifleWeapon::StopFireInternal()
{
	Cast<ABaseCharacter>(GetOwner())->GetWeaponComponent()->StopFire();
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