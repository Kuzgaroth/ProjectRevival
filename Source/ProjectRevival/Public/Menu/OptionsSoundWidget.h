// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsSoundWidget.generated.h"

class USoundCue;
class USlider;

UCLASS()
class PROJECTREVIVAL_API UOptionsSoundWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UPROPERTY(meta=(BindWidget))
	USlider* Slider_0;

	UFUNCTION()
	void OnMasterVolumeChange(float newValue);
	
	UPROPERTY(meta=(BindWidget))
	USlider* Slider;
	
	UFUNCTION()
	void OnEffectsVolumeChange(float newValue);

	UPROPERTY(meta=(BindWidget))
	USlider* Slider_1;

	UFUNCTION()
	void OnMusicVolumeChange(float newValue);

	UPROPERTY(meta=(BindWidget))
	USlider* Slider_2;

	UFUNCTION()
	void OnVoiceVolumeChange(float newValue);

	UFUNCTION()
	void SetVolume(float newValue, FString WhatSound);

	UPROPERTY()
	USoundMix* SoundMix;
private:
	UPROPERTY()
	class UPRGameInstance* MyGameInstance;

	void SaveSoundSettings();
	void LoadSoundSettings();
};
