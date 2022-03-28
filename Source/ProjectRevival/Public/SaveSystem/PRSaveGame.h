// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PRSaveGame.generated.h"

USTRUCT()
struct FCheckPointSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid CheckpointId;
};

USTRUCT()
struct FWeaponSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	float CurrentClips;

	UPROPERTY()
	float CurrentAmmo;
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	FGuid LastCheckpointReached;
	//WeaponsSaveData
	UPROPERTY()
	TArray<FWeaponSaveData> WeaponSaveDatas;
	
	//CharacterSaveData
	UPROPERTY()
	float HP;
};

UCLASS()
class PROJECTREVIVAL_API UPRSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FPlayerSaveData PlayerSaveData;

	UPROPERTY()
	TArray<FCheckPointSaveData> ReachedCheckpoints;
};
