// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "ConfirmationWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"

#include "OptionsWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void LeaveEvent();
	
protected:
	UPROPERTY(meta=(BindWidget))
	UButton* BackButton;

	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* OptionsWidgetSwitcher;

	UPROPERTY(meta=(BindWidget))
	UButton* ControlsButton;

	UPROPERTY(meta=(BindWidget))
	UButton* GraphicsButton;

	UPROPERTY(meta=(BindWidget))
	UButton* SoundButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* GameButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> OptionsControlsWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> OptionsSoundWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> OptionsGraphicsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> OptionsGameWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> ConfirmationWidgetClass;

	virtual void NativeOnInitialized() override;

private:
	UPROPERTY()
	UConfirmationWidget* ConfirmationWidget;
	
	UFUNCTION()
    void OnBack();

	UFUNCTION()
	void OnControls();

	UFUNCTION()
	void OnGraphics();

	UFUNCTION()
	void OnSound();

	UFUNCTION()
	void OnGame();

	UFUNCTION()
	void ChooseLevel();

	UFUNCTION()
	void OpenMenu();

	UFUNCTION()
	bool CheckUnsavedChanges() const;

	UFUNCTION()
	void ApplyAllChanges();

	UFUNCTION()
	void CloseConfirmationWidget();

	FTimerHandle TimerHandle;
};
