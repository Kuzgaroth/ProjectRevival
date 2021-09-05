// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoreTypes.h"
#include "ProjectRevival/Public/CoreTypes.h"

#include "LevelItemWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class PROJECTREVIVAL_API ULevelItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnLevelSelectedSignature OnLevelSelected;
	void SetLevelData(const FLevelData& NewLevelData);
	const FLevelData& GetLevelData() const {return LevelData;}

	void SetSelected(bool IsSelected);
	
protected:
	UPROPERTY(meta=(BindWidget))
	UButton* LevelSelectButton;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UImage* LevelImage = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	UImage* FrameImage;

	virtual void NativeOnInitialized() override;
private:
	UPROPERTY()
	FLevelData LevelData;

	UFUNCTION()
	void OnLevelItemClicked();

	UFUNCTION()
	void OnLevelItemHovered();

	UFUNCTION()
	void OnLevelItemUnHovered();
};
