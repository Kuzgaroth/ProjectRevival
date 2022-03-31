// Project Revival. All Rights Reserved


#include "Weapon/SoldierRifleWeapon.h"

#include "BaseCharacter.h"
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

// void ASoldierRifleWeapon::StartFire2(const FPlayerPositionData& PlayerPos)
// {
// 	InitFX();
// 	CurrentBurstShot = BurstClipsNumber * BurstBulletsNumber;
// 	CurrentBurstRow = BurstClipsNumber;
// 	OneRowTime = BurstBulletsDelay * BurstBulletsNumber + BurstClipsDelay;
// 	GetWorld()->GetTimerManager().SetTimer(ClipsTimerHandle, this, &ASoldierRifleWeapon::ShootRowInternal, OneRowTime, true);
// }

void ASoldierRifleWeapon::ShootRowInternal()
{
	--CurrentBurstRow;
	if (CurrentBurstRow <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Owner of SoldierRifle is: %s"), *GetOwner()->GetName());
		Cast<ABaseCharacter>(GetOwner())->GetWeaponComponent()->StopFire();
	}
	GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ASoldierRifleWeapon::MakeShotInternal, BurstBulletsDelay, true);
	MakeShotInternal();
}

void ASoldierRifleWeapon::MakeShotInternal()
{
	UE_LOG(LogTemp, Warning, TEXT("Current Burst Shot: %d"), CurrentBurstShot);
	MakeShot();
	--CurrentBurstShot;
	if (CurrentBurstShot <= 0)
	{
		StopFireInternal();
	}
}

void ASoldierRifleWeapon::StopFire()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(ClipsTimerHandle)) {return;}
	if (GetWorld()->GetTimerManager().IsTimerActive(ShotTimerHandle)) {GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);}
	GetWorld()->GetTimerManager().ClearTimer(ClipsTimerHandle);
	StoppedFireInWeaponDelegate.Broadcast();
	SetFXActive(false);
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