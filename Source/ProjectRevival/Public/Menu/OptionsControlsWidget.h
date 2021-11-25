// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "OptionsControlsWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UOptionsControlsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UScrollBox* KeyVerticalBox;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* ActionSizeBox;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* AxisSizeBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> KeyMapClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> AxisMapClass;

	virtual void NativeOnInitialized() override;
	
};
