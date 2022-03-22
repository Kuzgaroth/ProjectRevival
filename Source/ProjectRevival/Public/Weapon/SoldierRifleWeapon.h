// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/RifleWeapon.h"
#include "SoldierRifleWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoppedFireInWeapon);

UCLASS()
class PROJECTREVIVAL_API ASoldierRifleWeapon : public ARifleWeapon
{
	GENERATED_BODY()
	
public:
	virtual void StartFire() override;
	virtual void StopFire() override;
	
	FStoppedFireInWeapon StoppedFireInWeaponDelegate;

	//By default is true. If false than it will shoot in a common rifle non-stop way (which is yet quite not implemented in soldier lol)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMode")
	bool bUseBurst = true;

	//Number of shoot rows in a series
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMode")
	int32 BurstClipsNumber = 1;

	//Number of shots in one row
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMode")
	int32 BurstBulletsNumber = 10;

	//Delay between shoot rows
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMode")
	float BurstClipsDelay = 0.8;

	//Is completely equal to "TimeBetweenShots" but overrides it. So if they are different than head to the value set here
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMode")
	float BurstBulletsDelay = TimeBetweenShots;

protected:
	virtual void BeginPlay() override;
	
private:
	int8 CurrentBurstShot;
	int8 CurrentBurstRow;
	float OneRowTime;
	
	void MakeShotInternal();
	void ShootRowInternal();
	void StopFireInternal();
	
	FTimerHandle ShotTimerHandle;
	FTimerHandle ClipsTimerHandle;
};
