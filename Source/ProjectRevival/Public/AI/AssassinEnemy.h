// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "Player/BaseCharacter.h"
#include "AssassinEnemy.generated.h"

UCLASS()
class PROJECTREVIVAL_API AAssassinEnemy : public AAICharacter
{
	GENERATED_BODY()
public:
	bool IsAttacking() const;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsAttacking = false;
private:
	//
};
