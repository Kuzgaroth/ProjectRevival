// Project Revival. All Rights Reserved


#include "Interfaces/ICoverable.h"

bool FCoverData::IsInCover() const
{
	return CoverType==None || IsInCoverTransition ? false : true;
}

void FCoverData::StartCover(int8 CameraPos, int8 PartPos, ECoverType CType, AActor* CoverActor)
{
	IsInCoverTransition=true;
	CoverType = CType;
	CoverSide = CameraPos>0 ? Right : Left;
	CoverPart = PartPos==0 ? Middle : Edge;
	CoverObject = CoverActor;
}

void FCoverData::StopCover()
{
	IsInCoverTransition = true;
	CoverObject = nullptr;
}

void FCoverData::TurnStart(float Amount)
{
	if (Amount>0 && CoverSide==Left || Amount<0 && CoverSide==Right) IsTurning = true;
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
	if (IsInFireTransition) IsFiring = !IsFiring;
	IsTurning = IsSwitchingCoverType = IsInCoverTransition = IsInFireTransition = false;
	CoverType = CType;
	CoverSide = CSide;
	CoverPart = CPart;
	UE_LOG(LogTemp, Warning, TEXT("updating param is here"));
}

bool FCoverData::IsInTransition() const
{
	return IsInFireTransition || IsInCoverTransition || IsTurning || IsSwitchingCoverType;
}

bool FCoverData::IsFiringInCover() const
{
	return IsFiring;
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
