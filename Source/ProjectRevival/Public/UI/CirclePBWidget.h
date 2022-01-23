// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystem/PRAbilityTypes.h"
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
	EGASInputActions AbilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* InnerCircleFull;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* InnerCircleEmpty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UMaterialInterface* ExternalCircleMaterial;

	UFUNCTION(BlueprintCallable)
	void StartCooldown(float NewCooldownTime);

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void OnEndCooldown();

	EGASInputActions GetAbilityAction() const { return AbilityAction; }

	void SetAbilityAction(EGASInputActions NewAbilityAction) { AbilityAction = NewAbilityAction; }

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
	float CooldownTime;
	float Percent;
	bool bEnable;
};
