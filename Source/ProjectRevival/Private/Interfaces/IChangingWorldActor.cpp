// Project Revival. All Rights Reserved


#include "Interfaces/IChangingWorldActor.h"

// Add default functionality here for any IIChangingWorldObjCoverCheck functions that are not pure virtual.
bool IIChangingWorldActor::CheckIsChangeAbleObjIsCover()
{
	return false;
}

bool IIChangingWorldActor::TryToFindCoverPoint(FVector PlayerPos, FVector& CoverPos)
{
	return false;
}

