// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LauncherWeapon.h"
#include "Weapon/BaseWeapon.h"
#include "DimensionRevolver.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API ADimensionRevolver : public ALauncherWeapon
{
	GENERATED_BODY()
private:
	float HealPercent=30.0f;
public:
	virtual void MakeShot() override;
	void SetHealPercentToBullet(float Percent);
};
