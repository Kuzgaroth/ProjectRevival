// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class PROJECTREVIVAL_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetHealthPercent(float Percent);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* HealthProgressBar;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	float PercentVisibilityThreshold = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	float PercentColorThreshold = 0.3f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	FLinearColor GoodColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	FLinearColor BadColor = FLinearColor::Red;
};
