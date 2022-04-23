// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AmmoCrate.h"
#include "GameFramework/SaveGame.h"
#include "PRSaveGame.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPRSaveSystem, All, All)

USTRUCT()
struct FCheckPointSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FName CheckpointName;

	FCheckPointSaveData()
	{
		CheckpointName = NAME_None;
	}
	FCheckPointSaveData(FName Name)
	{
		CheckpointName = Name;
	}
};

USTRUCT()
struct FWeaponSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 CurrentClips;

	UPROPERTY()
	int32 CurrentAmmo;
	FWeaponSaveData()
	{
		CurrentAmmo = CurrentClips =0;
	}
	FWeaponSaveData(float Ammo, float Clips)
	{
		CurrentClips = Clips;
		CurrentAmmo = Ammo;
	}

	TArray<uint8> ByteArray;
};

USTRUCT()
struct FAmmoCrateSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 CurrentClips;

	FAmmoCrateSaveData()
	{
		CurrentClips = 0;
	}
	FAmmoCrateSaveData(int32 Clips)
	{
		CurrentClips = Clips;
	}

	TArray<uint8> ByteArray;
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	FCheckPointSaveData LastCheckpointReached;
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

	UPROPERTY()
	TMap<FName, FAmmoCrateSaveData> AmmoCrates;

	UPROPERTY()
	TEnumAsByte<EChangeWorld> WorldNum;

	UPROPERTY()
	TEnumAsByte<EGameDifficulty> GameDifficulty;

	UPROPERTY()
	bool InitialSave=true;
};
