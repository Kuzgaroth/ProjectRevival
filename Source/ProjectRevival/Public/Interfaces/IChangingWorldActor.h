// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IChangingWorldActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIChangingWorldActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTREVIVAL_API IIChangingWorldActor
{
	GENERATED_BODY()
	
public:
	virtual bool CheckIsChangeAbleObjIsCover();

	virtual bool TryToFindCoverPoint(FVector PlayerPos, FVector& CoverPos);
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
};
