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
public:
	UFUNCTION()
	bool HasUnsavedChanges();
	
	UFUNCTION()
	void ApplyChanges();
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyDefaultButton;
	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* UIRemoveCheckBox;

	UFUNCTION()
	void SetView();

	UFUNCTION()
    void ApplyDefaultChanges();

	virtual void NativeOnInitialized() override;
};
