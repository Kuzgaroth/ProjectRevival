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

	/**
	 * Changes a status (whether it's taken or not) of a CoverPoint (BoxComponent).
	 * @param bIsFree if false than there is already someone how wants to take it (i.e. other bots can't use it). if true than it is free and one can use it.
	 */
	virtual void SetLastCoverPointStatus(bool bIsFree);
};
