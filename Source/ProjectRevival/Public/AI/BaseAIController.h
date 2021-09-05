// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class UPRAIPerceptionComponent;
class URespawnComponent;

UCLASS()
class PROJECTREVIVAL_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABaseAIController();
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
