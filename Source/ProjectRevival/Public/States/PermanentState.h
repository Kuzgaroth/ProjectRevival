// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PermanentState.generated.h"

enum class EStateLayer;

DECLARE_MULTICAST_DELEGATE(FOnStateEndedSignature)
DECLARE_MULTICAST_DELEGATE(FOnStateStartedSignature);
DECLARE_DELEGATE_RetVal(bool, FCheckInternalDelegateSignature);

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
	EStateLayer StateLayer;
public:
	FOnStateEndedSignature StateEndDelegate;
	FOnStateStartedSignature StateStartDelegate;
	FCheckInternalDelegateSignature CheckInternalDelegate;
	void SetFriendlyStates(TArray<UPermanentState*>& Friends);
	bool IsStateFriendly(UPermanentState* CheckedState) const;
	void SetLayer(const EStateLayer Layer){StateLayer = Layer;}
	FORCEINLINE EStateLayer GetLayer() const{return StateLayer;}
};

UENUM()
enum class EStateLayer
{
	Movement=0,
	Action,
	Max
};

