// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PRPlayerState.generated.h"



UCLASS()
class PROJECTREVIVAL_API APRPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	int32 GetTeamId() const {return TeamId;}
	FLinearColor  GetTeamColor() const{ return  TeamColor;}
	int32 GetKillsNum() const{return KillsNum;}
	int32 GetDeathsNum() const{return DeathsNum;}
	void SetTeamId(int32 ID){TeamId = ID;}
	void SetTeamColor(const FLinearColor& NewColor){TeamColor=NewColor;}
	void AddKill() {++KillsNum;}
	void AddDeath() {++DeathsNum;}
	void LogInfo();
private:
	int32 TeamId;
	FLinearColor TeamColor;
	int32 KillsNum = 0;
	int32 DeathsNum = 0;
};
