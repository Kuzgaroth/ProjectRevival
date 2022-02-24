// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IChangingWorldObjCoverCheck.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIChangingWorldObjCoverCheck : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTREVIVAL_API IIChangingWorldObjCoverCheck
{
	GENERATED_BODY()
	
public:
	virtual bool CheckIsChangeAbleObjIsCover();

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
};
