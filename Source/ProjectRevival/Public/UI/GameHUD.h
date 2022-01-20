// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "GameHUD.generated.h"

UCLASS()
class PROJECTREVIVAL_API AGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	UUserWidget* GetPlayerHUDWidget() { return PlayerHUDWidget; }
	
	virtual void DrawHUD() override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void BeginPlay() override;
private:
	UPROPERTY()
	TMap<EMatchState, UUserWidget*> GameWidgets;

	UPROPERTY()
	UUserWidget* CurrentWidget = nullptr;

	UPROPERTY()
	UUserWidget* PlayerHUDWidget = nullptr;

	void DrawCrossHair();
	void OnMatchStateChanged(EMatchState NewMatchState);
};
