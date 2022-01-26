// Project Revival. All Rights Reserved


#include "Interfaces/ICoverable.h"

bool FCoverData::IsInCover() const
{
	return CoverType==None ? false : true;
}

void FCoverData::StartCover(int8 CameraPos, int8 PartPos, ECoverType CType, AActor* CoverActor)
{
	PendingCoverType = CType;
	PendingCoverSide = CameraPos>0 ? Right : Left;
	PendingCoverPart = PartPos==0 ? Middle : Edge;
	IsInCoverTransition=true;
	CoverObject = CoverActor;
}

void FCoverData::StopCover()
{
	IsInCoverTransition = true;
	CoverObject = nullptr;
}

void FCoverData::TurnStart()
{
	IsTurning = true;
}

void FCoverData::TurnEnd(ECoverSide NewSide)
{
	CoverSide = NewSide;
}

void FCoverData::TrySwitchCoverType(IICoverable* ICoverablePawn)
{
	ECoverType NewPossibleCover = ICoverablePawn->CheckCover();
	switch (CoverType)
	{
	case High:
		IsSwitchingCoverType = true;
		return;
	case Low:
		if (NewPossibleCover==High) IsSwitchingCoverType = true;
		return;
	default:
		break;
	}
	return;
}

void FCoverData::OnCoverStatusUpdated(ECoverType CType, ECoverSide CSide, ECoverPart CPart)
{
	CoverType = CType;
	CoverSide = CSide;
	CoverPart = CPart;
	IsTurning = IsSwitchingCoverType = IsInCoverTransition = IsInFireTransition = false;
}

FCoverData::FCoverData()
{
	CoverType = PendingCoverType = None;
	CoverPart = PendingCoverPart = CPNone;
	CoverSide = PendingCoverSide = CSNone;
	IsTurning = IsFiring = IsSwitchingCoverType = IsInCoverTransition = IsInFireTransition = false;
	CoverObject = nullptr;
}

// Add default functionality here for any IICoverable functions that are not pure virtual.
ECoverType IICoverable::CheckCover()
{
	return None;
}
