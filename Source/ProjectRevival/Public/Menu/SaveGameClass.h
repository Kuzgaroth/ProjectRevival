// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameClass.generated.h"

UCLASS()
class PROJECTREVIVAL_API USaveGameClass : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
    float SaveSlider_0Value;
    	
    UPROPERTY()
    float SaveSliderValue;
    
    UPROPERTY()
    float SaveSlider_1Value;
    
    UPROPERTY()
    float SaveSlider_2Value;
};
