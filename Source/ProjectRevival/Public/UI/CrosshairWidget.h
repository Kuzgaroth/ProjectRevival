// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

#include "CrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnHitDetected(bool IsInHead);
	
	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void PlayHitAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void PlayHeadHitAnimation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* CrosshairNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* CrosshairHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* CrosshairHitHead;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* CrosshairImage;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* DotImage;

	UFUNCTION(BlueprintCallable)
	void OnStopAnimation();

	virtual void NativeOnInitialized() override;
};
