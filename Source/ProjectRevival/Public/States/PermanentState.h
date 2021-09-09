// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PermanentState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStateEndedSignature)

UCLASS()
class PROJECTREVIVAL_API UPermanentState : public UObject
{
	GENERATED_BODY()
public:
	UPermanentState();
	~UPermanentState();

	virtual void StartState();
	virtual void EndState();
private:
	TArray<UPermanentState*> FriendlyStates;
public:
	FOnStateEndedSignature StateEndedDelegate;

	void SetFriendlyStates(TArray<UPermanentState*>& Friends);
	bool IsStateFriendly(UPermanentState* CheckedState);
};


