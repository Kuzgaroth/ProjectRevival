// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "GameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class PROJECTREVIVAL_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
protected:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* PLayerStatBox;

	UPROPERTY(meta=(BindWidget))
	UButton* ResetButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

	virtual void NativeOnInitialized() override;
private:
	void OnMatchStateChanged(EMatchState NewMatchState);
	void UpdatePlayersStat();

	UFUNCTION()
	void OnResetLevel();
};
