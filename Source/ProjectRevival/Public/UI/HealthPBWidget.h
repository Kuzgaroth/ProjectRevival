// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"


#include "HealthPBWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UHealthPBWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UMaterialInterface* ImageSegmented;

	UFUNCTION(BlueprintCallable)
	void ChangePercent(float NewPercent);

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void OnHealthFull();
	
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* ProgressImage;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* ProgressFull;
	
	UPROPERTY(BlueprintReadWrite)
    UMaterialInstanceDynamic* DMI;

	virtual void NativeOnInitialized() override;

private:
	float Percent;
};
