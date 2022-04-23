// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


#include "DifficultyItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UDifficultyItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	FString DifficultyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	FString Description;

	UPROPERTY(meta=(BindWidget))
	UButton* DifficultyButton;

	virtual void NativeOnInitialized() override;

protected:
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* DifficultyImage;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* DifficultyNameTextBlock;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* DescriptionTextBlock;

};
