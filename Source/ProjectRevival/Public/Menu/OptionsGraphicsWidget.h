// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "OptionsGraphicsWidget.generated.h"

class UCheckBox;
class UComboBoxString;
/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UOptionsGraphicsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	bool HasUnsavedChanges();

	UFUNCTION()
	void ApplyChanges();
	
protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResolutionBoxString;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* FullScreenCheckBox;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* VSyncCheckBox;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ShadowsBoxString;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* AntiAliasingBoxString;

	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyDefaultButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void SetView();

	UFUNCTION()
	void ApplyDefaultChanges();

	TArray<FIntPoint> PossibleResolutions;
};
