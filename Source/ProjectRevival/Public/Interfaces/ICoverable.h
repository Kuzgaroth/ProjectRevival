// Project Revival. All Rights Reserved

#pragma once

#define COVER_TRACE_CHANNEL ECC_GameTraceChannel3

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICoverable.generated.h"

// Cover structures and delegates

class IICoverable;

UENUM(BlueprintType)
enum ECoverType
{
	High,
	Low,
	None
};

UENUM(BlueprintType)
enum ECoverPart
{
	Middle,
	Edge,
	CPNone
};

UENUM(BlueprintType)
enum ECoverSide
{
	Left,
	Right,
	CSNone
};

USTRUCT(BlueprintType)
struct FCoverData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	TEnumAsByte<ECoverType> CoverType;
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	TEnumAsByte<ECoverPart> CoverPart;
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	TEnumAsByte<ECoverSide> CoverSide;
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	bool IsTurning;
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	bool IsFiring;
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	bool IsSwitchingCoverType;
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	bool IsInCoverTransition;
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	bool IsInFireTransition;
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	AActor* CoverObject;

	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	TEnumAsByte<ECoverType> PendingCoverType;
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	TEnumAsByte<ECoverSide> PendingCoverSide;
	UPROPERTY(BlueprintReadOnly, Category="CoverData")
	TEnumAsByte<ECoverPart> PendingCoverPart;
	bool IsInCover() const;
	void StartCover(int8 CameraPos, int8 PartPos, ECoverType CType, AActor* CoverActor);
	void StopCover();
	bool TurnStart(float Amount);
	void TurnEnd(ECoverSide NewSide);
	void TrySwitchCoverType(IICoverable* ICoverablePawn);
	void OnCoverStatusUpdated(ECoverType CType, ECoverSide CSide, ECoverPart CPart);
	bool IsInTransition() const;
	bool IsReadyToFire() const;
	void CoverToAim();
	void AimToCover();
	bool TryMoveInCover(float Amount, const AActor* Player);
	void AdjustCameraInCover(IICoverable* CoverableObject);
	FCoverData();
	void SetOwner(AActor* PlayerOwner);
private:
	UPROPERTY()
	AActor* Owner = nullptr;
};

UINTERFACE(MinimalAPI)
class UICoverable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTREVIVAL_API IICoverable
{
	GENERATED_BODY()
public:
	virtual ECoverType CheckCover();
	virtual void OnTurn();
	virtual void OnAimInCover();
};
