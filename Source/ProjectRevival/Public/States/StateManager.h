// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"
#include "StateManager.generated.h"

class UPermanentState;

UCLASS()
class PROJECTREVIVAL_API UStateManager : public UObject
{
	GENERATED_BODY()
public:
	UStateManager();
	virtual ~UStateManager() override;
private:
	UPROPERTY()
	UPermanentState* CurrentState;
	UPROPERTY()
	UPermanentState* DefaultState;
	
	void OnStateTimeFinished();
public:
	bool TryEnterState(UPermanentState* NewState);
	void EnterState(UPermanentState* NewState);
	void SetDefaultState(UPermanentState* DefaultState);
	void EnterDefaultState();
};
