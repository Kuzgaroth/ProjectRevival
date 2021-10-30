// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectRevival/Public/CoreTypes.h"

#include "MenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UPRGameInstance;
class ULevelItemWidget;
class USoundCue;

UCLASS()
class PROJECTREVIVAL_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta=(BindWidget))
	UButton* ContinueGameButton;

	UPROPERTY(meta=(BindWidget))
	UButton* NewGameButton;

	UPROPERTY(meta=(BindWidget))
	UButton* LoadGameButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* OptionsButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta=(BindWidget))
	UButton* CreditsButton;

	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* LevelsBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> OptionsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> CreditsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	USoundCue* StartGameSound;
	
	virtual void NativeOnInitialized() override;
private:
	UPROPERTY()
	TArray<ULevelItemWidget*> LevelItemWidgets;
	
	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnContinueGame();

	UFUNCTION()
	void OnNewGame();

	UFUNCTION()
	void OnOptions();

	UFUNCTION()
	void OnLoadGame();
	
	UFUNCTION()
	void OnQuitGame();

	UFUNCTION()
	void OnCredits();
	

	void InitLevelItems();
	void OnLevelSelected(const FLevelData& NewLevelData);
};
