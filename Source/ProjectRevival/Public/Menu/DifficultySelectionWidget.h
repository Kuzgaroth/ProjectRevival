// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "DifficultyItemWidget.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"

#include "DifficultySelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UDifficultySelectionWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	UButton* BackButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(meta=(BindWidget))
	UDifficultyItemWidget* LowDifficultyWidget;

	UPROPERTY(meta=(BindWidget))
	UDifficultyItemWidget* MediumDifficultyWidget;

	UPROPERTY(meta=(BindWidget))
	UDifficultyItemWidget* HighDifficultyWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	USoundCue* StartGameSound;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnBack();

	UFUNCTION()
	void OnLowDifficultyPressed();
	
	UFUNCTION()
	void OnMediumDifficultyPressed();

	UFUNCTION()
	void OnHighDifficultyPressed();

	void SetDifficulty(FString DifficultyName);

	void OpenLevel();
};
