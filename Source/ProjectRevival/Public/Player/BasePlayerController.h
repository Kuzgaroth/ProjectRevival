// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "BasePlayerController.generated.h"

class URespawnComponent;

UCLASS()
class PROJECTREVIVAL_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABasePlayerController();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Respawn")
	URespawnComponent* RespawnComponent;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
private:
	void OnPauseGame();
	void OnMatchStateChanged(EMatchState NewMatchState);
};
