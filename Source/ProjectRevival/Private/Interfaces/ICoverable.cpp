// Project Revival. All Rights Reserved


#include "Interfaces/ICoverable.h"

#include "Kismet/KismetSystemLibrary.h"

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

	TryMoveInCover(CameraPos,Owner);
}

void FCoverData::StopCover()
{
	IsInCoverTransition = true;
	CoverObject = nullptr;
}

bool FCoverData::TurnStart(float Amount)
{
	if (IsTurning) return true;
	if (Amount>0 && CoverSide==Left || Amount<0 && CoverSide==Right)
	{
		if (Owner)
		{
			auto Coverable = Cast<IICoverable>(Owner);
			if (Coverable)
			{
				Coverable->OnTurn();	
			}
		}
		IsTurning = true;
	}
	return IsTurning;
}

void FCoverData::TurnEnd(ECoverSide NewSide)
{
	CoverSide = NewSide;
}

bool FCoverData::TrySwitchCoverType(IICoverable* ICoverablePawn)
{
	ECoverType NewPossibleCover = ICoverablePawn->CheckCover();
	if (IsInTransition()) return false;
	switch (CoverType)
	{
	case High:
		IsSwitchingCoverType = true;
		return true;
	case Low:
		if (NewPossibleCover==High)
		{
			IsSwitchingCoverType = true;
			return true;
		}
		return false;
	default:
		break;
	}
	return false;
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

bool FCoverData::IsReadyToFire() const
{
	return IsFiring && IsInCover();
}

void FCoverData::CoverToAim()
{
	if (CoverPart== Middle && CoverType==High) return;
	IsInFireTransition = true;
}

void FCoverData::AimToCover()
{
	IsInFireTransition = true;
}

bool FCoverData::TryMoveInCover(float Amount, const AActor* Player)
{
	const bool TurnProcess = TurnStart(Amount);
	const float ForwardOffset = 100.f;
	const float RightOffset = CoverType == Low ? 60.f : 45.f;
	FHitResult HitResut;
	if (!Player) return false;
	const auto WingStart = Player->GetActorRightVector()*Amount*RightOffset+Player->GetActorLocation();
	const auto WingEnd = WingStart+Player->GetActorForwardVector()*ForwardOffset;
	const bool NotEndOfCover = UKismetSystemLibrary::LineTraceSingle(Player->GetWorld(), WingStart, WingEnd,
		UEngineTypes::ConvertToTraceType(COVER_TRACE_CHANNEL),false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, HitResut, true);
	CoverPart = NotEndOfCover ? Middle : Edge;
	if (TurnProcess)
	{
		return TurnProcess;
	}
	return NotEndOfCover;
}

FCoverData::FCoverData()
{
	CoverType = PendingCoverType = None;
	CoverPart = PendingCoverPart = CPNone;
	CoverSide = PendingCoverSide = CSNone;
	IsTurning = IsFiring = IsSwitchingCoverType = IsInCoverTransition = IsInFireTransition = false;
	CoverObject = nullptr;
}

void FCoverData::SetOwner(AActor* PlayerOwner)
{
	Owner = PlayerOwner;
}

void FCoverData::AdjustCameraInCover(IICoverable* CoverableObject)
{
	
}

// Add default functionality here for any IICoverable functions that are not pure virtual.
ECoverType IICoverable::CheckCover()
{
	return None;
}

void IICoverable::OnTurn() {}

void IICoverable::OnAimInCover() {}
