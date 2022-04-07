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
	virtual void MakeShot() override;
	
};
