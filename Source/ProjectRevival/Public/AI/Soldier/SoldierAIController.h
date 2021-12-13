// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SoldierAIController.generated.h"

class UPRAIPerceptionComponent;
class URespawnComponent;

/*
 *   Контроллер бота-солдата
 */
UCLASS()
class PROJECTREVIVAL_API ASoldierAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASoldierAIController();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UPRAIPerceptionComponent* PRPerceptionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	FName FocusOnKeyName = "EnemyActor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Respawn")
	URespawnComponent* RespawnComponent;
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
private:
	AActor* GetFocusOnActor();
};
