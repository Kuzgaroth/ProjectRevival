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
	UFUNCTION()
	bool HasUnsavedChanges();

	UFUNCTION()
    void ApplyChanges();
	
protected:
	UPROPERTY(meta=(BindWidget))
	USlider* MasterVolumeSlider;

	UFUNCTION()
	void OnMasterVolumeChange(float newValue);
	
	UPROPERTY(meta=(BindWidget))
	USlider* EffectVolumeSlider;
	
	UFUNCTION()
	void OnEffectsVolumeChange(float newValue);

	UPROPERTY(meta=(BindWidget))
	USlider* MusicVolumeSlider;

	UFUNCTION()
	void OnMusicVolumeChange(float newValue);

	UPROPERTY(meta=(BindWidget))
	USlider* VoiceVolumeSlider;

	UFUNCTION()
	void OnVoiceVolumeChange(float newValue);

	UFUNCTION()
	void SetVolume(float newValue, FString WhatSound);

	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyDefaultButton;

	UFUNCTION()
	void SetView();
	
	UFUNCTION()
	void ApplyDefaultChanges();

	virtual void NativeOnInitialized() override;
};
