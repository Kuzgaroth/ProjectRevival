// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

#include "CirclePBWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UCirclePBWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	float CooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* IconImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* InnerCircleFull;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* InnerCircleEmpty;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UMaterialInterface* ExternalCircleMaterial;

	UFUNCTION(BlueprintCallable)
	void StartCooldown();

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void OnEndCooldown();
	
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* ExternalCircle;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* InnerCircle;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* DMI;

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void EndCooldown();
	
private:
	float Percent;
	bool bEnable;
};
