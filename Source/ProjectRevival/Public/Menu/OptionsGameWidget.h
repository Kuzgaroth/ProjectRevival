// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CheckBox.h"

#include "OptionsGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UOptionsGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UCheckBox* UIRemoveCheckBox;
	
};
