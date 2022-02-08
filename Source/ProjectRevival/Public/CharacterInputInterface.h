// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTREVIVAL_API ICharacterInputInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
};
