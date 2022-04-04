// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.h"
#include "Engine/GameInstance.h"
#include "PRGameInstance.generated.h"



UCLASS()
class PROJECTREVIVAL_API UPRGameInstance : public UGameInstance
{
	GENERATED_BODY()

	void Init() override;

	FString SaveSlot = "SoundData";

	UPROPERTY()
	class USaveGameClass* SoundData;

public:
	void SaveSoundData(float SaveSlider_0Value, float SaveSliderValue, float SaveSlider_1Value, float SaveSlider_2Value);
	class USaveGameClass* LoadSoundData();
	
	FLevelData GetStartupLevel() const { return StartupLevel;}
	void SetStartupLevel(const FLevelData& LevelData) { StartupLevel = LevelData;}
	FName GetMenuLevelName() const { return MenuLevelName;}
	const TArray<FLevelData>& GetLevelsData() const {return LevelsData;}
protected:
	UPROPERTY(EditDefaultsOnly, Category="Maps", meta=(ToolTip = "Names are Unique!"))
	TArray<FLevelData> LevelsData;

	UPROPERTY(EditDefaultsOnly, Category="Maps")
	FName MenuLevelName = NAME_None;

private:
	
	FLevelData StartupLevel;
};
