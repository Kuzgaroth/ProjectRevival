// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PROJECTREVIVAL_API UPlayerStatRowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetPlayerName(const FText& Text);
	void SetKills(const FText& Text);
	void SetDeaths(const FText& Text);
	void SetTeam(const FText& Text);
	void SetPlayerIndicatorVisibility(bool Visibile);
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayerNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* KillsTextBlock;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DeathsTextBlock;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TeamTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	UImage* PlayerIndicatorImage;
};
