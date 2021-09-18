// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PermanentState.h"
#include "TemporaryState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStateInterraptedSignature)
DECLARE_MULTICAST_DELEGATE(FOnStateTimeFinishedSignature)

UCLASS()
class PROJECTREVIVAL_API UTemporaryState : public UPermanentState
{
	GENERATED_BODY()
public:
	UTemporaryState();
	virtual ~UTemporaryState() override;

	virtual void EndState() override;
	virtual void StartState() override;

	virtual void OnStateTimeEnded();
private:
	bool IsInProgress;
public:
	FOnStateInterraptedSignature StateInterraptedDelegate;
	FOnStateTimeFinishedSignature StateTimeFinishedDelegate;
};
