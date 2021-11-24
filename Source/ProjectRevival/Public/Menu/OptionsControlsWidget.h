// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	UVerticalBox* KeyVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> KeyMapClass;

	virtual void NativeOnInitialized() override;
	
private:
	
};
