// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PermanentState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStateEndedSignature)
DECLARE_MULTICAST_DELEGATE(FOnStateStartedSignature);

UCLASS()
class PROJECTREVIVAL_API UPermanentState : public UObject
{
	GENERATED_BODY()
public:
	UPermanentState();
	virtual ~UPermanentState() override;

	virtual void StartState();
	virtual void EndState();
private:
	TArray<UPermanentState*> FriendlyStates;
public:
	FOnStateEndedSignature StateEndedDelegate;
	FOnStateStartedSignature StateStartDelegate;
	void SetFriendlyStates(TArray<UPermanentState*>& Friends);
	bool IsStateFriendly(UPermanentState* CheckedState) const;
};


