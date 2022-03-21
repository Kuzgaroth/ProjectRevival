#pragma once
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/ICoverable.h"
#include "CoreTypes.generated.h"

class UBoxComponent;
//Weapon
class ABaseWeapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ABaseWeapon*);

// Log Categories
DECLARE_LOG_CATEGORY_EXTERN(LogPRAISystem, Log, All);

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo", meta=(EditCondition="!bInfiniteAmmo"))
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo")
	bool bInfiniteAmmo;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UAnimMontage* ReloadAnimMontage;
};

//Health
DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float);

//UI

USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	UTexture2D* CrossHairIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	UTexture2D* MainIcon;
};

USTRUCT(BlueprintType)
struct FDecalData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	FVector Size = FVector(10.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	float LifeTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	float FadeOutTime = 0.7f;
};

class UNiagaraSystem;
class USoundCue;

USTRUCT(BlueprintType)
struct FImpactData
{
	GENERATED_BODY()

	//if set to "true" then Niagara is used, otherwise uses Cascade. By default is set to "true"
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	bool bUseNiagaraImpactEffect = true;
	
	//Niagara effect to play
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* NiagaraEffect;

	//Cascade effect to play
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	UParticleSystem* CascadeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	FDecalData DecalData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	USoundCue* Sound;
};

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta=(ClampMin="1", ClampMax="100"))
	int32 PlayersNum = 2;
};

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	WaitingToStart = 0,
	InProgress,
	Pause,
	GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, EMatchState);

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
	FName LevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
	FName LevelDisplayName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
	UTexture2D* LevelThumb;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);

USTRUCT(BlueprintType)
struct FPlayerAimZoomBlueprint
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	UCurveVector* CurveVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	FVector Offset = FVector(120.0, 60.0, 0.0);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	FVector CoverHighOffset = FVector(170.0, 120.0, 0.0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	FVector CoverLowOffset = FVector(130.0, 20.0, 20.0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	FVector CoverLowMiddleOffset = FVector(70.0, -83.0, 70.0);
	
	UPROPERTY()
	FVector StartLoc;
	
	UPROPERTY()
	FVector EndLoc;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	UCurveFloat* CurveFloat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	float FieldOfView = 60.0;
	
	UPROPERTY()
	float CurrentFieldOfView;
	
	UPROPERTY()
	FVector StartStartPos = FVector(0.0, 0.0, 0.0);
	
	UPROPERTY()
	bool IsZooming = false;
	
};


struct FLeftSideViewBlueprint;
struct FCameraCover;
class UCameraCoverFunctions;

UCLASS()
class PROJECTREVIVAL_API UPlayerAimZoomFunctions : public UObject
{
	GENERATED_BODY()

public:
	void TimelineFieldOfView(float, FPlayerAimZoomBlueprint&);
	void TimelineProgress(float, FPlayerAimZoomBlueprint&);
	
	void CameraZoomIn(USpringArmComponent*&, FLeftSideViewBlueprint&, FPlayerAimZoomBlueprint&, UCameraComponent*&, FTimeline&, FCoverData&, FCameraCover&, UCameraCoverFunctions*&);
	void CameraZoomOut(USpringArmComponent*&, FTimeline&, FPlayerAimZoomBlueprint&, FCoverData&);
	
	USpringArmComponent* *LocalSpringArmComponent = nullptr;
	UCameraComponent* *LocalCameraComponent = nullptr;

	FTimeline CurveTimeline;
};

USTRUCT(BlueprintType)
struct FLeftSideViewBlueprint
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	UCurveFloat* LeftSideFloat;

	UPROPERTY()
	bool Block = false;

	UPROPERTY()
	bool IsMoving = false;

	UPROPERTY()
	bool CamPos = false;

	UPROPERTY()
	float EndPos = 0.0;

	UPROPERTY()
	float StartPos = 0.0;

	UPROPERTY()
	float Proverka = 0.0;
	
	UPROPERTY()
    bool Repeat = false;

	UPROPERTY()
	FVector SavePosRight = FVector(0.0);

	UPROPERTY()
	FVector SavePosLeft = FVector(0.0);
};

UCLASS()
class PROJECTREVIVAL_API ULeftSideViewFunctions : public UObject
{

	GENERATED_BODY()

public:

	USpringArmComponent* *LocalSpringArmComponent = nullptr;
	FLeftSideViewBlueprint *LocalLeftSideViewBlueprintLocal = nullptr;

	FTimeline LeftSideViewCurveTimeline;

	void TimelineLeftSideView(float, FLeftSideViewBlueprint&, FPlayerAimZoomBlueprint&);
	void CameraStop(FLeftSideViewBlueprint&, FPlayerAimZoomBlueprint&);
	void CameraBlock();
	void OnCameraMove(USpringArmComponent*&, UCameraComponent*&, FLeftSideViewBlueprint& LocalLeftSideViewBlueprint, FTimeline& LeftSideViewCurveTimeline, FCameraCover&, FCoverData&);
};

USTRUCT(BlueprintType)
struct FCameraCover
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	UCurveVector* CoverVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	FVector CameraCover = FVector(80.0, 0.0, -10.0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	float Low = 70.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	UCurveFloat* CoverFloat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	float FieldOfView = 60.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	float CoverZoomFieldOfView = 60.0;
	
	UPROPERTY()
	float CurrentFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	UCurveFloat* CoverYShiftCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	float CoverYShift = 30.0;

	UPROPERTY()
	float StartPos;

	UPROPERTY()
	float EndPos;

	UPROPERTY()
	bool bIsShift = false;
	
	UPROPERTY()
	bool IsShifting = false;
	
	UPROPERTY()
    FVector SavePosRight;

	UPROPERTY()
	FVector SavePosLeft;

	UPROPERTY()
	bool bIsTurning = false;
};

USTRUCT(BlueprintType)
struct FCoverPointsAndPossibility
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="CoverPointsData")
	int CoverPointsAmount;
	
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="CoverPointsData")
	TArray<USceneComponent*> CoverPositions;

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="CoverPointsData")
	bool CanBeTakenAsCover=true;

	TMap<UBoxComponent*,bool> PointIsNotTaken;
	TArray<FVector> PositionsOfCoverPoints;
	
};

UCLASS()
class PROJECTREVIVAL_API UCameraCoverFunctions : public UObject
{
	GENERATED_BODY()
public:
	FTimeline CameraCoverTimeline;
	FTimeline CameraCoverYShiftTimeline;

	FVector Start;
	FVector End;

	ECoverType CoverType;

	void TimelineCover(float, UCameraCoverFunctions*&, USpringArmComponent*&, FCameraCover&, FLeftSideViewBlueprint&);
	void TimelineCoverFieldOfView(float, UCameraComponent*&, FCameraCover&);
	void TimelineCoverYShift(float, USpringArmComponent*&, FCameraCover&);
};


inline void UPlayerAimZoomFunctions::TimelineFieldOfView(float Value, FPlayerAimZoomBlueprint& LocalPlayerAimZoomBlueprint)
{
	float NewFieldOfView = FMath::Lerp((*LocalCameraComponent)->FieldOfView, LocalPlayerAimZoomBlueprint.CurrentFieldOfView, Value);
	(*LocalCameraComponent)->FieldOfView = NewFieldOfView;
	if ((*LocalCameraComponent)->FieldOfView >= LocalPlayerAimZoomBlueprint.CurrentFieldOfView && LocalPlayerAimZoomBlueprint.CurrentFieldOfView == 90.0) LocalPlayerAimZoomBlueprint.IsZooming = false;
}

inline void UPlayerAimZoomFunctions::TimelineProgress(float Value, FPlayerAimZoomBlueprint& LocalPlayerAimZoomBlueprint)
{
	FVector NewLocation = FMath::Lerp(LocalPlayerAimZoomBlueprint.StartLoc, LocalPlayerAimZoomBlueprint.EndLoc, Value);
	(*LocalSpringArmComponent)->SocketOffset = NewLocation;
}

inline void UPlayerAimZoomFunctions::CameraZoomIn(USpringArmComponent*& SpringArmComponent, FLeftSideViewBlueprint& LeftSideViewBlueprint, FPlayerAimZoomBlueprint& LocalPlayerAimZoomBlueprint, UCameraComponent*& CameraComponent, FTimeline& LocalCurveTimeline, FCoverData& CoverData, FCameraCover& CameraCover, UCameraCoverFunctions*& CameraCoverFunctions)
{
	LocalCameraComponent = &CameraComponent;
	LocalSpringArmComponent = &SpringArmComponent;
	
	if (LocalPlayerAimZoomBlueprint.StartStartPos == FVector(0.0, 0.0, 0.0)) LocalPlayerAimZoomBlueprint.StartStartPos = SpringArmComponent->SocketOffset;
	SpringArmComponent->SocketOffset = LocalPlayerAimZoomBlueprint.StartStartPos;

	LocalPlayerAimZoomBlueprint.StartLoc = SpringArmComponent->SocketOffset;
	if (CoverData.IsInCover())
	{
		if (CameraCoverFunctions->CoverType == ECoverType::High) LocalPlayerAimZoomBlueprint.EndLoc = FVector(SpringArmComponent->SocketOffset.X + LocalPlayerAimZoomBlueprint.CoverHighOffset.X, SpringArmComponent->SocketOffset.Y, SpringArmComponent->SocketOffset.Z + LocalPlayerAimZoomBlueprint.CoverHighOffset.Z);
		else if (CameraCoverFunctions->CoverType == ECoverType::Low && CameraCover.bIsShift == true) LocalPlayerAimZoomBlueprint.EndLoc = FVector(SpringArmComponent->SocketOffset.X + LocalPlayerAimZoomBlueprint.CoverLowOffset.X, SpringArmComponent->SocketOffset.Y, SpringArmComponent->SocketOffset.Z + LocalPlayerAimZoomBlueprint.CoverLowOffset.Z);
		else if (CameraCoverFunctions->CoverType == ECoverType::Low && CameraCover.bIsShift == false) LocalPlayerAimZoomBlueprint.EndLoc = FVector(SpringArmComponent->SocketOffset.X + LocalPlayerAimZoomBlueprint.CoverLowMiddleOffset.X, SpringArmComponent->SocketOffset.Y, SpringArmComponent->SocketOffset.Z + LocalPlayerAimZoomBlueprint.CoverLowMiddleOffset.Z);
	}
	else LocalPlayerAimZoomBlueprint.EndLoc = FVector(SpringArmComponent->SocketOffset.X + LocalPlayerAimZoomBlueprint.Offset.X, SpringArmComponent->SocketOffset.Y, SpringArmComponent->SocketOffset.Z + LocalPlayerAimZoomBlueprint.Offset.Z);
	if (LeftSideViewBlueprint.CamPos == false)
	{
		if (!(CoverData.IsInCover())) LocalPlayerAimZoomBlueprint.EndLoc.Y -= LocalPlayerAimZoomBlueprint.Offset.Y / 1.2;
		else if (CameraCoverFunctions->CoverType == ECoverType::High) LocalPlayerAimZoomBlueprint.EndLoc.Y -= LocalPlayerAimZoomBlueprint.CoverHighOffset.Y / 18.0;
		else if (CameraCoverFunctions->CoverType == ECoverType::Low && CameraCover.bIsShift == true) LocalPlayerAimZoomBlueprint.EndLoc.Y += LocalPlayerAimZoomBlueprint.CoverLowOffset.Y / 1.2;
		else if (CameraCoverFunctions->CoverType == ECoverType::Low && CameraCover.bIsShift == false) LocalPlayerAimZoomBlueprint.EndLoc.Y += LocalPlayerAimZoomBlueprint.CoverLowMiddleOffset.Y / 1.2;
	}
	else if (!(CoverData.IsInCover())) LocalPlayerAimZoomBlueprint.EndLoc.Y += LocalPlayerAimZoomBlueprint.Offset.Y / 2.0;
	else if (CameraCoverFunctions->CoverType == ECoverType::High) LocalPlayerAimZoomBlueprint.EndLoc.Y -= LocalPlayerAimZoomBlueprint.CoverHighOffset.Y / 2.8;
		else if (CameraCoverFunctions->CoverType == ECoverType::Low && CameraCover.bIsShift == true) LocalPlayerAimZoomBlueprint.EndLoc.Y -= LocalPlayerAimZoomBlueprint.CoverLowOffset.Y * 2.5;
			else if (CameraCoverFunctions->CoverType == ECoverType::Low && CameraCover.bIsShift == false) LocalPlayerAimZoomBlueprint.EndLoc.Y -= LocalPlayerAimZoomBlueprint.CoverLowMiddleOffset.Y / 2.0;
	if (CameraCover.bIsShift == true && LeftSideViewBlueprint.CamPos == false) LocalPlayerAimZoomBlueprint.EndLoc.Y -= CameraCover.CoverYShift;
	else if (CameraCover.bIsShift == true && LeftSideViewBlueprint.CamPos == true) LocalPlayerAimZoomBlueprint.EndLoc.Y += CameraCover.CoverYShift;
	if (!(CoverData.IsInCover())) LocalPlayerAimZoomBlueprint.CurrentFieldOfView = LocalPlayerAimZoomBlueprint.FieldOfView;
	else LocalPlayerAimZoomBlueprint.CurrentFieldOfView = CameraCover.CoverZoomFieldOfView;

	LocalPlayerAimZoomBlueprint.IsZooming = true;
	LocalCurveTimeline.PlayFromStart();
	
}

inline void UPlayerAimZoomFunctions::CameraZoomOut(USpringArmComponent*& SpringArmComponent, FTimeline& LocalCurveTimeline, FPlayerAimZoomBlueprint& LocalPlayerAimZoomBlueprint, FCoverData& CoverData)
{
	LocalPlayerAimZoomBlueprint.EndLoc = LocalPlayerAimZoomBlueprint.StartLoc;
	LocalPlayerAimZoomBlueprint.StartLoc = SpringArmComponent->SocketOffset;
	if (!(CoverData.IsInCover())) LocalPlayerAimZoomBlueprint.CurrentFieldOfView = 90.0;
	else LocalPlayerAimZoomBlueprint.CurrentFieldOfView = LocalPlayerAimZoomBlueprint.FieldOfView;

	LocalPlayerAimZoomBlueprint.IsZooming = false;
	LocalCurveTimeline.PlayFromStart();
}


inline void ULeftSideViewFunctions::TimelineLeftSideView(float Value, FLeftSideViewBlueprint& LocalLeftSideViewBlueprint, FPlayerAimZoomBlueprint& LocalPlayerAimZoomBlueprint)
{
	float NewView = FMath::Lerp(LocalLeftSideViewBlueprint.StartPos, LocalLeftSideViewBlueprint.EndPos, Value);
	(*LocalSpringArmComponent)->SocketOffset.Y = NewView;
	if (((*LocalSpringArmComponent)->SocketOffset.Y >= LocalLeftSideViewBlueprint.EndPos && LocalLeftSideViewBlueprint.CamPos == true || (*LocalSpringArmComponent)->SocketOffset.Y <= LocalLeftSideViewBlueprint.EndPos && LocalLeftSideViewBlueprint.CamPos == false) && LocalLeftSideViewBlueprint.Repeat == false) { CameraStop(LocalLeftSideViewBlueprint, LocalPlayerAimZoomBlueprint); LocalLeftSideViewBlueprint.Repeat = true; }
}

inline void ULeftSideViewFunctions::CameraStop(FLeftSideViewBlueprint& LocalLeftSideViewBlueprint, FPlayerAimZoomBlueprint& LocalPlayerAimZoomBlueprint)
{
	FTimerHandle TimerCameraBlock;
	LocalLeftSideViewBlueprint.IsMoving = false;
	GetWorld()->GetTimerManager().SetTimer(TimerCameraBlock, this, &ULeftSideViewFunctions::CameraBlock, 0.5, false);
	if (LocalLeftSideViewBlueprint.CamPos == true)
	{
		LocalLeftSideViewBlueprint.CamPos = false;
		(*LocalSpringArmComponent)->SocketOffset.Y = LocalLeftSideViewBlueprint.Proverka;
	}
	else LocalLeftSideViewBlueprint.CamPos = true;
	LocalPlayerAimZoomBlueprint.StartStartPos = (*LocalSpringArmComponent)->SocketOffset;
}

inline void ULeftSideViewFunctions::CameraBlock()
{
	LocalLeftSideViewBlueprintLocal->Block = false;
}

inline void ULeftSideViewFunctions::OnCameraMove(USpringArmComponent*& SpringArmComponent, UCameraComponent*& CameraComponent, FLeftSideViewBlueprint& LocalLeftSideViewBlueprint, FTimeline& LocalLeftSideViewCurveTimeline, FCameraCover& CameraCover, FCoverData& CoverData)
{
	LocalSpringArmComponent = &SpringArmComponent;
	LocalLeftSideViewBlueprintLocal = &LocalLeftSideViewBlueprint;

	CameraCover.bIsTurning = false;
	if (!(CoverData.IsInCover()))
		if (LocalLeftSideViewBlueprint.CamPos == false) SpringArmComponent->SocketOffset = CameraCover.SavePosRight;
		else SpringArmComponent->SocketOffset = CameraCover.SavePosLeft;
	
	if (LocalLeftSideViewBlueprint.CamPos == false) LocalLeftSideViewBlueprint.Proverka = SpringArmComponent->SocketOffset.Y;
	
	if (CoverData.IsInCover())
	{
		if (LocalLeftSideViewBlueprint.CamPos == false)
		{
			if (LocalLeftSideViewBlueprint.SavePosRight == FVector(0.0)) LocalLeftSideViewBlueprint.SavePosRight = SpringArmComponent->SocketOffset;
			else SpringArmComponent->SocketOffset = LocalLeftSideViewBlueprint.SavePosRight;
		}
		else if (LocalLeftSideViewBlueprint.SavePosLeft == FVector(0.0)) LocalLeftSideViewBlueprint.SavePosLeft = SpringArmComponent->SocketOffset;
			else SpringArmComponent->SocketOffset = LocalLeftSideViewBlueprint.SavePosLeft;
	}

	LocalLeftSideViewBlueprint.StartPos = SpringArmComponent->SocketOffset.Y;
	LocalLeftSideViewBlueprint.EndPos = LocalLeftSideViewBlueprint.StartPos - (SpringArmComponent->SocketOffset.Y + tan(CameraComponent->GetRelativeRotation().Yaw * PI / 180) * SpringArmComponent->TargetArmLength) * 2.f;
	if (CameraCover.bIsShift == true && LocalLeftSideViewBlueprint.CamPos == false)
	{
		LocalLeftSideViewBlueprint.EndPos += CameraCover.CoverYShift;
		CameraCover.bIsShift = false;
	}
	else if (CameraCover.bIsShift == true && LocalLeftSideViewBlueprint.CamPos == true)
	{
		LocalLeftSideViewBlueprint.EndPos -= CameraCover.CoverYShift;
		CameraCover.bIsShift = false;
	}
	LocalLeftSideViewBlueprint.Block = true;
	LocalLeftSideViewBlueprint.IsMoving = true;
	LocalLeftSideViewBlueprint.Repeat = false;
	LocalLeftSideViewCurveTimeline.PlayFromStart();
}


inline void UCameraCoverFunctions::TimelineCover(float Value, UCameraCoverFunctions*& CameraCoverFunctions, USpringArmComponent*& SpringArmComponent, FCameraCover& CameraCover, FLeftSideViewBlueprint& LocalLeftSideViewBlueprint)
{
	FVector NewLocation = FMath::Lerp(CameraCoverFunctions->Start, CameraCoverFunctions->End, Value);
	SpringArmComponent->SocketOffset = NewLocation;
}


inline void UCameraCoverFunctions::TimelineCoverFieldOfView(float Value, UCameraComponent*& CameraComponent, FCameraCover& CameraCover)
{
	float NewFieldOfView = FMath::Lerp(CameraComponent->FieldOfView, CameraCover.CurrentFieldOfView, Value);
	CameraComponent->FieldOfView = NewFieldOfView;
}


inline void UCameraCoverFunctions::TimelineCoverYShift(float Value, USpringArmComponent*& SpringArmComponent, FCameraCover& CameraCover)
{
	float NewView = FMath::Lerp(CameraCover.StartPos, CameraCover.EndPos, Value);
	SpringArmComponent->SocketOffset.Y = NewView;
	if (abs(NewView) >= abs(CameraCover.EndPos)) CameraCover.IsShifting = false;
}


DECLARE_LOG_CATEGORY_EXTERN(LogPRAIDecorators, Log, All);

struct FBTPlayerCheckDecoratorMemory
{
	bool bLastRawResult;
};

UENUM(BlueprintType)
enum class EWing: uint8
{
	Left = 0,
	Center = 1,
	Right = 2
};

USTRUCT(BlueprintType)
struct FPlayerPositionData
{
	GENERATED_BODY()
private:
	UPROPERTY()
	AActor* PlayerActor;
	UPROPERTY()
	AActor* PlayerCover;
public:
	FPlayerPositionData(AActor* PActor, AActor* PCover)
	{
		PlayerActor = PActor;
		PlayerCover = PCover;
	}
	FPlayerPositionData()
	{
		PlayerActor=nullptr;
		PlayerCover=nullptr;
	}
	void SetActor(AActor* PActor)
	{
		PlayerActor = PActor;
	}
	AActor* GetActor() const
	{
		return PlayerActor;
	}
	void SetCover(AActor* PCover)
	{
		PlayerCover = PCover;
	}
	AActor* GetCover() const
	{
		return PlayerCover;
	}
	FVector GetActorPosition() const
	{
		return (PlayerActor) ? PlayerActor->GetActorLocation() : FVector(0.0, 0.0, 0.0);
	}
	FORCEINLINE void operator=(const FPlayerPositionData& PlayerPos)
	{
		PlayerActor = PlayerPos.PlayerActor;
		PlayerCover = PlayerPos.PlayerCover;
	}
};
