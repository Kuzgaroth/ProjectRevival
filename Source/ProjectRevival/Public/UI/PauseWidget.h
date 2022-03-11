// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu/ConfirmationWidget.h"

#include "PauseWidget.generated.h"

class UButton;

UCLASS()
class PROJECTREVIVAL_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:


protected:
	UPROPERTY(meta=(BindWidget))
	UButton* ClearPauseButton;

	UPROPERTY(meta=(BindWidget))
	UButton* CheckPointButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* PlayAgainButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* OptionsButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> OptionsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> ConfirmationWidgetClass;


	virtual void NativeOnInitialized() override;
private:
	UPROPERTY()
	UConfirmationWidget* ConfirmationWidget;
	
	UFUNCTION()
	void OnClearPause();

	UFUNCTION()
	void OnOptions();

	UFUNCTION()
	void OnCheckPoint();

	UFUNCTION()
    void OnPlayAgain();

	UFUNCTION()
	void OnQuitGame();

	UFUNCTION()
	void LoadCheckPoint();

	UFUNCTION()
	void PlayAgain();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void CloseConfirmationWidget();
};
