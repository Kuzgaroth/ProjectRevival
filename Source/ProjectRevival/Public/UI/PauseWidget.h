// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	UButton* SaveGameButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* LoadGameButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* OptionsButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> OptionsWidgetClass;


	virtual void NativeOnInitialized() override;
private:
	UFUNCTION()
	void OnClearPause();

	UFUNCTION()
	void OnOptions();

	UFUNCTION()
	void OnSaveGame();

	UFUNCTION()
    void OnLoadGame();

	UFUNCTION()
	void OnQuitGame();

	
};
