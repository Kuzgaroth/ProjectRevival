#pragma once
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CoreTypes.generated.h"

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta=(ClampMin="1", ClampMax="10"))
	int32 RoundsNum = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta=(ClampMin="3", ClampMax="300"))
	int32 RoundTime = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Teams")
	FLinearColor DefaultTeamColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Teams")
	TArray<FLinearColor> TeamColors;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta=(ClampMin="3", ClampMax="20"))
    int32 RespawnTime = 1;
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

UCLASS()
class PROJECTREVIVAL_API UPlayerAimZoomFunctions : public UObject
{
	GENERATED_BODY()

public:
	void TimelineFieldOfView(float, FPlayerAimZoomBlueprint&);
	void TimelineProgress(float, FPlayerAimZoomBlueprint&);
	
	void CameraZoomIn(USpringArmComponent*&, FLeftSideViewBlueprint&, FPlayerAimZoomBlueprint&, UCameraComponent*&, FTimeline&);
	void CameraZoomOut(USpringArmComponent*&, FTimeline&, FPlayerAimZoomBlueprint&);
	
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
	void OnCameraMove(USpringArmComponent*&, UCameraComponent*&, FLeftSideViewBlueprint& LocalLeftSideViewBlueprint, FTimeline& LeftSideViewCurveTimeline);
};

inline void UPlayerAimZoomFunctions::TimelineFieldOfView(float Value, FPlayerAimZoomBlueprint& LocalPlayerAimZoomBlueprint)
{
	UE_LOG(LogTemp, Warning, TEXT("Progress"));
	float NewFieldOfView = FMath::Lerp((*LocalCameraComponent)->FieldOfView, LocalPlayerAimZoomBlueprint.CurrentFieldOfView, Value);
	(*LocalCameraComponent)->FieldOfView = NewFieldOfView;
	if ((*LocalCameraComponent)->FieldOfView >= LocalPlayerAimZoomBlueprint.CurrentFieldOfView && LocalPlayerAimZoomBlueprint.CurrentFieldOfView == 90.0) LocalPlayerAimZoomBlueprint.IsZooming = false;
}

inline void UPlayerAimZoomFunctions::TimelineProgress(float Value, FPlayerAimZoomBlueprint& LocalPlayerAimZoomBlueprint)
{
	FVector NewLocation = FMath::Lerp(LocalPlayerAimZoomBlueprint.StartLoc, LocalPlayerAimZoomBlueprint.EndLoc, Value);
	(*LocalSpringArmComponent)->SocketOffset = NewLocation;
}

inline void UPlayerAimZoomFunctions::CameraZoomIn(USpringArmComponent*& SpringArmComponent, FLeftSideViewBlueprint& LeftSideViewBlueprint, FPlayerAimZoomBlueprint& LocalPlayerAimZoomBlueprint, UCameraComponent*& CameraComponent, FTimeline& LocalCurveTimeline)
{
	LocalCameraComponent = &CameraComponent;
	LocalSpringArmComponent = &SpringArmComponent;
	
	if (LocalPlayerAimZoomBlueprint.StartStartPos == FVector(0.0, 0.0, 0.0)) LocalPlayerAimZoomBlueprint.StartStartPos = SpringArmComponent->SocketOffset;
	SpringArmComponent->SocketOffset = LocalPlayerAimZoomBlueprint.StartStartPos;

	LocalPlayerAimZoomBlueprint.StartLoc = SpringArmComponent->SocketOffset;
	LocalPlayerAimZoomBlueprint.EndLoc = FVector(SpringArmComponent->SocketOffset.X + LocalPlayerAimZoomBlueprint.Offset.X, SpringArmComponent->SocketOffset.Y, SpringArmComponent->SocketOffset.Z + LocalPlayerAimZoomBlueprint.Offset.Z);
	if (LeftSideViewBlueprint.CamPos == false) LocalPlayerAimZoomBlueprint.EndLoc.Y -= LocalPlayerAimZoomBlueprint.Offset.Y; else LocalPlayerAimZoomBlueprint.EndLoc.Y += LocalPlayerAimZoomBlueprint.Offset.Y / 2.0;
	LocalPlayerAimZoomBlueprint.CurrentFieldOfView = LocalPlayerAimZoomBlueprint.FieldOfView;

	LocalPlayerAimZoomBlueprint.IsZooming = true;
	LocalCurveTimeline.PlayFromStart();
	
}

inline void UPlayerAimZoomFunctions::CameraZoomOut(USpringArmComponent*& SpringArmComponent, FTimeline& LocalCurveTimeline, FPlayerAimZoomBlueprint& LocalPlayerAimZoomBlueprint)
{
	LocalPlayerAimZoomBlueprint.EndLoc = LocalPlayerAimZoomBlueprint.StartLoc;
	LocalPlayerAimZoomBlueprint.StartLoc = SpringArmComponent->SocketOffset;
	LocalPlayerAimZoomBlueprint.CurrentFieldOfView = 90.0;

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

inline void ULeftSideViewFunctions::OnCameraMove(USpringArmComponent*& SpringArmComponent, UCameraComponent*& CameraComponent, FLeftSideViewBlueprint& LocalLeftSideViewBlueprint, FTimeline& LocalLeftSideViewCurveTimeline)
{
	LocalSpringArmComponent = &SpringArmComponent;
	LocalLeftSideViewBlueprintLocal = &LocalLeftSideViewBlueprint;
	
	if (LocalLeftSideViewBlueprint.CamPos == false) LocalLeftSideViewBlueprint.Proverka = SpringArmComponent->SocketOffset.Y;

	LocalLeftSideViewBlueprint.StartPos = SpringArmComponent->SocketOffset.Y;
	LocalLeftSideViewBlueprint.EndPos = LocalLeftSideViewBlueprint.StartPos - (SpringArmComponent->SocketOffset.Y + tan(CameraComponent->GetRelativeRotation().Yaw * PI / 180) * SpringArmComponent->TargetArmLength) * 2.f;
	LocalLeftSideViewBlueprint.Block = true;
	LocalLeftSideViewBlueprint.IsMoving = true;
	LocalLeftSideViewBlueprint.Repeat = false;
	LocalLeftSideViewCurveTimeline.PlayFromStart();
} 

DECLARE_LOG_CATEGORY_EXTERN(LogPRAIDecorators, Log, All);

struct FBTPlayerCheckDecoratorMemory
{
	bool bLastRawResult;
};
